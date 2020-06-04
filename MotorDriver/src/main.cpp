#include <Arduino.h>

#define A1 4
#define B1 2

void setup() {
  pinMode(A1,OUTPUT);
  pinMode(B1,OUTPUT);
}

void loop() {
  digitalWrite(A1, LOW);
  digitalWrite(B1, HIGH);
  delay(2000);
  digitalWrite(A1, HIGH);
  digitalWrite(B1, LOW);
  delay(2000);
  digitalWrite(A1, LOW);
  digitalWrite(B1, LOW);
  delay(2000);
}