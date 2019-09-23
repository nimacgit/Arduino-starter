#include <Arduino.h>

void setup() {
 
  Serial.begin(9600); // setup serial
 
}
 
void loop() { 
  Serial.println(analogRead(A0));

  delay(100);

}