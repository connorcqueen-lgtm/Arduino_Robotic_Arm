
#include <Servo.h>

// Create servo objects
Servo baseServo;
Servo shoulderServo;
Servo gripperServo;

// Joystick pins
const int joystickX = A0;
const int joystickY = A1;
const int joystickButton = 2;

// Servo pins
const int basePin = 9;
const int shoulderPin = 10;
const int gripperPin = 11;

// Starting positions
int baseAngle = 90;
int shoulderAngle = 90;

// Temporary gripper positions
int gripperOpen = 60;
int gripperClosed = 110;

bool gripperIsOpen = true;
bool lastButtonState = HIGH;

// Joystick dead zone
const int deadZoneLow = 450;
const int deadZoneHigh = 570;

void setup() {
  baseServo.attach(basePin);
  shoulderServo.attach(shoulderPin);
  gripperServo.attach(gripperPin);

  pinMode(joystickButton, INPUT_PULLUP);

  baseServo.write(baseAngle);
  shoulderServo.write(shoulderAngle);
  gripperServo.write(gripperOpen);

  Serial.begin(9600);
}

void loop() {
  // Read joystick
  int xValue = analogRead(joystickX);
  int yValue = analogRead(joystickY);

  // Control base
  if (xValue < deadZoneLow) {
    baseAngle--;
  }
  else if (xValue > deadZoneHigh) {
    baseAngle++;
  }

  // Control shoulder
  if (yValue < deadZoneLow) {
    shoulderAngle--;
  }
  else if (yValue > deadZoneHigh) {
    shoulderAngle++;
  }

  // Limit servo angles
  baseAngle = constrain(baseAngle, 0, 180);
  shoulderAngle = constrain(shoulderAngle, 0, 180);

  baseServo.write(baseAngle);
  shoulderServo.write(shoulderAngle);

  // Read joystick button
  bool buttonState = digitalRead(joystickButton);

  // Toggle gripper on each button press
  if (lastButtonState == HIGH && buttonState == LOW) {
    gripperIsOpen = !gripperIsOpen;

    if (gripperIsOpen) {
      gripperServo.write(gripperOpen);
    }
    else {
      gripperServo.write(gripperClosed);
    }

    delay(50); // Simple button debounce
  }

  lastButtonState = buttonState;

  // Debugging
  Serial.print("Base: ");
  Serial.print(baseAngle);
  Serial.print(" Shoulder: ");
  Serial.print(shoulderAngle);
  Serial.print(" Gripper: ");
  Serial.println(gripperIsOpen ? "Open" : "Closed");

  delay(20);
}

