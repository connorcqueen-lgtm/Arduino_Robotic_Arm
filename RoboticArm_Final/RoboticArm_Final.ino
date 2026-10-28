
#include <Servo.h>
#include <Stepper.h>

// ==========================================
// ROBOTIC ARM FINAL PROJECT
// Arduino Uno
// 3 Servos + 28BYJ-48 Stepper
// Joystick + HC-SR04 + Tilt Switch + LED
// ==========================================

// ----- SERVO MOTORS -----
Servo baseServo;
Servo auxiliaryServo;
Servo gripperServo;

const int baseServoPin = 9;
const int auxiliaryServoPin = 10;
const int gripperServoPin = 11;

// Set to true only if the third servo is installed
// on an additional moving joint.
const bool useAuxiliaryServo = false;

// ----- STEPPER MOTOR -----
// Wiring: IN1=D3, IN2=D8, IN3=D12, IN4=D13
const int stepsPerRevolution = 2048;

Stepper shoulderStepper(
  stepsPerRevolution,
  3, 12, 8, 13
);

// Approximate software travel limits.
// These must be adjusted after mechanical testing.
long shoulderPosition = 0;
const long shoulderMin = -200;
const long shoulderMax = 200;

const int shoulderStepSize = 2;

// ----- JOYSTICK -----
const int joyX = A0;
const int joyY = A1;
const int joyButton = 2;

const int deadZoneLow = 450;
const int deadZoneHigh = 570;

// ----- SERVO POSITIONS -----
int baseAngle = 90;
int auxiliaryAngle = 90;

// Conservative initial limits.
// Adjust only after checking the physical joints.
const int baseMin = 60;
const int baseMax = 120;

const int auxiliaryMin = 60;
const int auxiliaryMax = 120;

const int gripperOpen = 60;
const int gripperClosed = 110;

bool gripperIsClosed = false;

// ----- ULTRASONIC SENSOR -----
const int trigPin = 6;
const int echoPin = 7;

const float warningDistance = 15.0;

// ----- TILT SWITCH -----
const int tiltPin = 4;

// ----- WARNING LED -----
const int warningLedPin = 5;

// ----- TIMING -----
unsigned long lastMovementTime = 0;
unsigned long lastSensorTime = 0;
unsigned long lastButtonTime = 0;

const unsigned long movementInterval = 30;
const unsigned long sensorInterval = 250;
const unsigned long debounceInterval = 200;

bool lastButtonState = HIGH;

bool objectNearby = false;
bool tiltWarning = false;

// ==========================================
// SETUP
// ==========================================

void setup() {
  Serial.begin(9600);

  // Configure joystick
  pinMode(joyButton, INPUT_PULLUP);

  // Configure ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);

  // Configure tilt switch
  pinMode(tiltPin, INPUT_PULLUP);

  // Configure warning LED
  pinMode(warningLedPin, OUTPUT);
  digitalWrite(warningLedPin, LOW);

  // Attach servo motors
  baseServo.attach(baseServoPin);
  gripperServo.attach(gripperServoPin);

  if (useAuxiliaryServo) {
    auxiliaryServo.attach(auxiliaryServoPin);
    auxiliaryServo.write(auxiliaryAngle);
  }

  // Set starting servo positions
  baseServo.write(baseAngle);
  gripperServo.write(gripperOpen);

  // Configure stepper
  shoulderStepper.setSpeed(8);

  Serial.println("Robotic Arm Initialized");
  Serial.println("Joystick X: Base");
  Serial.println("Joystick Y: Shoulder Stepper");
  Serial.println("Joystick Button: Gripper");
  Serial.println("Sensors: Warning LED");
}

// ==========================================
// MAIN LOOP
// ==========================================

void loop() {
  unsigned long currentTime = millis();

  // Read joystick button continuously
  controlGripper(currentTime);

  // Move motors periodically
  if (currentTime - lastMovementTime >= movementInterval) {
    lastMovementTime = currentTime;

    controlBase();
    controlShoulder();
  }

  // Read sensors periodically
  if (currentTime - lastSensorTime >= sensorInterval) {
    lastSensorTime = currentTime;

    checkUltrasonic();
    checkTilt();
    updateWarningLED();
    displaySensorStatus();
  }
}

// ==========================================
// BASE SERVO CONTROL
// ==========================================

void controlBase() {
  int xValue = analogRead(joyX);

  if (xValue < deadZoneLow) {
    baseAngle--;
  }
  else if (xValue > deadZoneHigh) {
    baseAngle++;
  }

  baseAngle = constrain(
    baseAngle,
    baseMin,
    baseMax
  );

  baseServo.write(baseAngle);
}

// ==========================================
// SHOULDER STEPPER CONTROL
// ==========================================

void controlShoulder() {
  int yValue = analogRead(joyY);

  int direction = 0;

  if (yValue < deadZoneLow) {
    direction = -1;
  }
  else if (yValue > deadZoneHigh) {
    direction = 1;
  }

  if (direction == 0) {
    return;
  }

  long targetPosition =
    shoulderPosition + direction * shoulderStepSize;

  // Prevent movement outside configured limits
  if (targetPosition < shoulderMin ||
      targetPosition > shoulderMax) {
    return;
  }

  shoulderStepper.step(direction * shoulderStepSize);

  shoulderPosition = targetPosition;
}

// ==========================================
// GRIPPER CONTROL
// ==========================================

void controlGripper(unsigned long currentTime) {
  bool buttonState = digitalRead(joyButton);

  if (lastButtonState == HIGH &&
      buttonState == LOW &&
      currentTime - lastButtonTime >= debounceInterval) {

    gripperIsClosed = !gripperIsClosed;

    if (gripperIsClosed) {
      gripperServo.write(gripperClosed);
    }
    else {
      gripperServo.write(gripperOpen);
    }

    lastButtonTime = currentTime;
  }

  lastButtonState = buttonState;
}

// ==========================================
// ULTRASONIC SENSOR
// ==========================================

void checkUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  unsigned long duration =
    pulseIn(echoPin, HIGH, 25000UL);

  if (duration == 0) {
    objectNearby = false;
    return;
  }

  float distance = duration * 0.0343 / 2.0;

  objectNearby =
    distance > 0 &&
    distance < warningDistance;
}

// ==========================================
// TILT SWITCH
// ==========================================

void checkTilt() {
  tiltWarning = digitalRead(tiltPin) == LOW;
}

// ==========================================
// WARNING LED
// ==========================================

void updateWarningLED() {
  if (objectNearby || tiltWarning) {
    digitalWrite(warningLedPin, HIGH);
  }
  else {
    digitalWrite(warningLedPin, LOW);
  }
}

// ==========================================
// SERIAL MONITOR DEBUGGING
// ==========================================

void displaySensorStatus() {
  Serial.print("Base: ");
  Serial.print(baseAngle);

  Serial.print(" | Shoulder steps: ");
  Serial.print(shoulderPosition);

  Serial.print(" | Object: ");
  Serial.print(objectNearby ? "NEAR" : "CLEAR");

  Serial.print(" | Tilt: ");
  Serial.print(tiltWarning ? "ACTIVE" : "NORMAL");

  Serial.print(" | LED: ");
  Serial.println(
    (objectNearby || tiltWarning) ? "ON" : "OFF"
  );
}
