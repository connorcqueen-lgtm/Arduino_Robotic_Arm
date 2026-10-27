#include <Servo.h>

// Servo motors
Servo baseServo;
Servo shoulderServo;
Servo gripperServo;

// Joystick pins
const int joyX = A0;
const int joyY = A1;
const int joyButton = 2;

// Sensor pins
const int trigPin = 6;
const int echoPin = 7;
const int tiltPin = 4;
const int ledPin = 5;

// Servo positions
int baseAngle = 90;
int shoulderAngle = 90;

const int gripperOpen = 60;
const int gripperClosed = 110;
bool gripperIsClosed = false;

// Button debounce
bool lastButtonState = HIGH;
unsigned long lastButtonTime = 0;

// Sensor timing
unsigned long lastSensorTime = 0;
const unsigned long sensorInterval = 250;

bool objectNearby = false;
bool tiltWarning = false;

void setup() {
  Serial.begin(9600);

  baseServo.attach(9);
  shoulderServo.attach(10);
  gripperServo.attach(11);

  baseServo.write(baseAngle);
  shoulderServo.write(shoulderAngle);
  gripperServo.write(gripperOpen);

  pinMode(joyButton, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(tiltPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  digitalWrite(trigPin, LOW);
  digitalWrite(ledPin, LOW);

  Serial.println("Robotic Arm Ready");
}

void loop() {
  // Read joystick
  int xValue = analogRead(joyX);
  int yValue = analogRead(joyY);

  // Move base
  if (xValue < 450) {
    baseAngle--;
  } else if (xValue > 570) {
    baseAngle++;
  }

  // Move shoulder
  if (yValue < 450) {
    shoulderAngle--;
  } else if (yValue > 570) {
    shoulderAngle++;
  }

  baseAngle = constrain(baseAngle, 0, 180);
  shoulderAngle = constrain(shoulderAngle, 0, 180);

  baseServo.write(baseAngle);
  shoulderServo.write(shoulderAngle);

  // Toggle gripper when joystick button is pressed
  bool buttonState = digitalRead(joyButton);

  if (lastButtonState == HIGH &&
      buttonState == LOW &&
      millis() - lastButtonTime > 200) {

    gripperIsClosed = !gripperIsClosed;

    gripperServo.write(
      gripperIsClosed ? gripperClosed : gripperOpen
    );

    lastButtonTime = millis();
  }

  lastButtonState = buttonState;

  // Read sensors periodically
  if (millis() - lastSensorTime >= sensorInterval) {
    lastSensorTime = millis();

    // Trigger ultrasonic measurement
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration =
      pulseIn(echoPin, HIGH, 30000UL);

    float distance = duration * 0.0343 / 2.0;

    // No echo is not treated as a nearby object
    objectNearby = duration > 0 && distance < 15.0;

    // Tilt switch warning
    tiltWarning = digitalRead(tiltPin) == LOW;

    // Warning LED: either sensor can activate it
    digitalWrite(
      ledPin,
      (objectNearby || tiltWarning) ? HIGH : LOW
    );

    Serial.print("Distance: ");

    if (duration > 0) {
      Serial.print(distance);
      Serial.print(" cm");
    } else {
      Serial.print("No echo");
    }

    Serial.print(" | Tilt: ");
    Serial.print(tiltWarning ? "CLOSED" : "OPEN");

    Serial.print(" | Warning: ");
    Serial.println(
      (objectNearby || tiltWarning) ? "ON" : "OFF"
    );
  }

  delay(20);
}