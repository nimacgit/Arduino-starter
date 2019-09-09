#include "DHT.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>

File myFile;
unsigned long time;
int collector = 0;
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
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  SD.remove("TermoHu.txt");
}


void loop() {
  h = DHT_sens.readHumidity();
  t = DHT_sens.readTemperature();
  int moisValue = analogRead(MOIS_PIN);
  int mois2Value = analogRead(MOIS2_PIN);
  int mois3Value = analogRead(MOIS3_PIN);

//  Serial.print (t,1);                  // one decimal
//  Serial.print (h,0);                  // zero decimal
  collector = (collector + 1)%2000;
  if(collector % 2000 == 0){
    myFile = SD.open("TermoHu.txt", FILE_WRITE);
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(t);
    lcd.print("C ");
    lcd.print("H:");
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("M:");
    lcd.print(moisValue);
    lcd.print(" ");
    lcd.print(mois2Value);
    lcd.print(" ");
    lcd.print(mois3Value);
    if (myFile) {
      Serial.println(time);
      Serial.print("T:");
      Serial.print(t);
      Serial.print("C ");
      Serial.print("H:");
      Serial.print(h);
      Serial.println("%");
      Serial.print("M:");
      Serial.print(moisValue);
      Serial.print(" ");
      Serial.print(mois2Value);
      Serial.print(" ");
      Serial.println(mois3Value);
      
      Serial.print("Writing to TermoHu.txt...");
      time = millis();
      myFile.println(time);
      myFile.print(t);
      myFile.print("C ");
      myFile.println(h);
      myFile.print("M1: ");
      myFile.println(moisValue);
      myFile.print("M2: ");
      myFile.println(mois2Value);
      myFile.print("M3: ");
      myFile.println(mois3Value);
      myFile.close();
      Serial.println("done.");
    } else {
      Serial.println("error opening test.txt");
    }
  }

  
}




/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
 */
