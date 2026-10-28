#include <Stepper.h>

// Steps per output shaft revolution (approximate)
const int stepsPerRevolution = 2048;

// Arduino pins connected to ULN2003
Stepper armStepper(
  stepsPerRevolution,
  3, 12, 8, 13
);

void setup() {
  Serial.begin(9600);

  armStepper.setSpeed(10);

  Serial.println("Stepper test started");
}

void loop() {
  Serial.println("Clockwise");
  armStepper.step(512);

  delay(1000);

  Serial.println("Counterclockwise");
  armStepper.step(-512);

  delay(1000);
}