// Robotic Arm - Tilt Switch Test

const int tiltPin = 4;
const int ledPin = 5;

void setup() {
  Serial.begin(9600);

  // Use Arduino's internal pull-up resistor
  pinMode(tiltPin, INPUT_PULLUP);

  // LED output
  pinMode(ledPin, OUTPUT);

  Serial.println("Tilt Switch Test Started");
}

void loop() {
  // Read the tilt switch
  int tiltState = digitalRead(tiltPin);

  if (tiltState == LOW) {
    // Switch contacts are closed
    digitalWrite(ledPin, HIGH);
    Serial.println("Switch CLOSED - LED ON");
  }
  else {
    // Switch contacts are open
    digitalWrite(ledPin, LOW);
    Serial.println("Switch OPEN - LED OFF");
  }

  delay(250);
}