void setup() {
  pinMode(13, OUTPUT);    // sets the digital pin 13 as output
  Serial.begin(9600);
}

void loop() {
  digitalWrite(3, HIGH); // sets the digital pin 13 on
  delay(1000);            // waits for a second
  digitalWrite(3, LOW);  // sets the digital pin 13 off
  delay(1000);            // waits for a second
}
