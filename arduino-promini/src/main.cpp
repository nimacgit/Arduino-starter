#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>

#define DHT11_PIN 8
#define MOIS_PIN A1
#define MOIS2_PIN A2
#define MOIS3_PIN A3

File myFile;
DHT DHT_sens(DHT11_PIN, DHT11);
int period = 5000;
unsigned long time_old = 0;
int redPin = 7;
int greenPin = 5;
float h, t;

void setup()
{
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  DHT_sens.begin();
  Serial.begin (9600);
  Serial.print("Initializing SD card...");
  while(!SD.begin(4)){
    Serial.println("initialization failed!");
    delay(1000);
  }
  Serial.println("initialization done.");
  SD.remove("TermoHu.txt");
  time_old = millis();
}

void loop() {
  h = DHT_sens.readHumidity();
  t = DHT_sens.readTemperature();
  int moisValue = analogRead(MOIS_PIN);
  int mois2Value = analogRead(MOIS2_PIN);
  int mois3Value = analogRead(MOIS3_PIN);

//  Serial.print (t,1);                  // one decimal
//  Serial.print (h,0);                  // zero decimal
  if(millis() > time_old + period){
    time_old = millis();
    myFile = SD.open("TermoHu.txt", FILE_WRITE);
    if (myFile) {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      Serial.println(time_old);
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
      
      myFile.println(time_old);
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
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      Serial.println("error opening test.txt");
    }
  }
}
