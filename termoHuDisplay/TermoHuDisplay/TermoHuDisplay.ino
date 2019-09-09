#include "DHT.h"
#include <LiquidCrystal.h>

float h, t;
#define DHT11_PIN 8
#define MOIS_PIN A1
#define MOIS2_PIN A2
#define MOIS3_PIN A3
DHT DHT_sens(DHT11_PIN, DHT11);           //datapin sensor to pin 10 Arduino

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Clears the LCD screen
  lcd.clear();
  DHT_sens.begin();
  Serial.begin (9600);
}


void loop() {
  h = DHT_sens.readHumidity();
  t = DHT_sens.readTemperature();
  
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);
  lcd.print("C ");
//  Serial.print (t,1);                  // one decimal
//  Serial.print (h,0);                  // zero decimal
  lcd.print("H:");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(0, 1);
  int moisValue = analogRead(MOIS_PIN);
  int mois2Value = analogRead(MOIS2_PIN);
  int mois3Value = analogRead(MOIS3_PIN);
  lcd.print("M:");
  lcd.print(moisValue);
  lcd.print(" ");
  lcd.print(mois2Value);
  lcd.print(" ");
  lcd.print(mois3Value);
  delay (1000);
}
