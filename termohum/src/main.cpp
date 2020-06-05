#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include <DHT.h>
#include <LiquidCrystal.h>

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

LiquidCrystal  lcd(RsPIN, EnPIN, D4PIN, D5PIN, D6PIN, D7PIN);
#define DHTPIN 8

DHT dht = DHT(DHTPIN, DHT11);

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  dht.begin();
  lcd.clear();
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity(); 
  lcd.setCursor(0, 0);
  lcd.print("");
  lcd.print(t);
  lcd.print("C ");
  lcd.print(h);
  lcd.print("%");

  // Printing the results on the serial monitor
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("    Humidity = ");
  Serial.print(h);
  Serial.println(" % ");
  delay(2000);
}