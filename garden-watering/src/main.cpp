#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal.h>

#define RELAYPIN 8
#define MOIS1_PIN A0
#define MOIS2_PIN A1
#define MOIS3_PIN A2
/*
LCD: 5V
K - GND
A - 5V
Rw - GND
V0 - potentio
VLD - 5V
VSS - GND
*/
#define RsPIN 2
#define EnPIN 3
#define D4PIN 4
#define D5PIN 5
#define D6PIN 6
#define D7PIN 7
#define DRYMOISMIN 400
#define DRYMOISMAX 600

int mois1Value;
int mois2Value;
int mois3Value;
LiquidCrystal  lcd(RsPIN, EnPIN, D4PIN, D5PIN, D6PIN, D7PIN);

void setup(){
  Serial.begin(9600);
  pinMode(RELAYPIN, OUTPUT);
  lcd.begin(16,2);
  lcd.clear();
}

void loop() {
  mois1Value = analogRead(MOIS1_PIN);
  mois2Value = analogRead(MOIS2_PIN);
  mois3Value = analogRead(MOIS3_PIN);
  lcd.setCursor(0, 0);
  lcd.print(mois1Value);
  lcd.print("-");
  lcd.print(mois2Value);
  lcd.print("-");
  lcd.print(mois3Value);

  Serial.print(mois1Value);
  Serial.print("\n");
  Serial.print(mois2Value);
  Serial.print("\n");
  Serial.print(mois3Value);
  Serial.print("\n");
  if((mois1Value < DRYMOISMAX && mois1Value > DRYMOISMIN) || (mois2Value < DRYMOISMAX && mois2Value > DRYMOISMIN) || (mois3Value < DRYMOISMAX && mois3Value > DRYMOISMIN)){
    digitalWrite(RELAYPIN, LOW);
  }
  else{
    digitalWrite(RELAYPIN,HIGH);
  }
  delay(1000);

}