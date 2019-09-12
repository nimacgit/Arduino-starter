#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

/*
 * SD card: 5V
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 DHT: 5V

 moisSensor: 3.3V
*/

#define DHT11_PIN 8
#define MOIS_PIN A1
#define MOIS2_PIN A2
#define MOIS3_PIN A3
#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 7
#define interruptPin 2

File myFile;
DHT DHT_sens(DHT11_PIN, DHT11);

int period = 5000;
unsigned long time_old = 0;
float h, t;
bool write2sd = false;
int moisValue;
int mois2Value;
int mois3Value;
char fileName[] = "TermoHu";
int lastFileNum = -1;

void writeToFile();
void changeSD();
void printState();
void ejectState();
void errorState();
void okState();
void writeState();

void setup()
{
  pinMode(GREENPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  errorState();
  DHT_sens.begin();
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeSD, FALLING);
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  while(!SD.begin(4)){
    Serial.println("initialization failed!");
    delay(1000);
  }
  Serial.println("initialization done.");
  time_old = millis();
}

void loop() {
  h = DHT_sens.readHumidity();
  t = DHT_sens.readTemperature();
  moisValue = analogRead(MOIS_PIN);
  mois2Value = analogRead(MOIS2_PIN);
  mois3Value = analogRead(MOIS3_PIN);

  if((millis() > time_old + period)){
    time_old = millis();
    if(!write2sd){
      ejectState();
    }
    else{
      writeToFile();
    }
  }
}

void openfile(){
  int num = 0;
  char strNum[5];
  char strName[16] = "";
  sprintf(strNum, "%d", num);
  strcpy(strName, fileName);
  strcat(strName, strNum);
  while(SD.exists(strName) && (num < lastFileNum || lastFileNum == -1)){
    num++;
    sprintf(strNum, "%d", num);
    strcpy(strName, fileName);
    strcat(strName, strNum);
  }
  sprintf(strNum, "%d", num);
  strcpy(strName, fileName);
  strcat(strName, strNum);
  lastFileNum = num;
  myFile = SD.open(strName, FILE_WRITE);
}

void writeToFile(){
  openfile();
  if (myFile) {
    writeState();
    
    // printState();

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
    okState();
  } else {
    errorState();
    Serial.println("error opening file");
  }
}

void printState(){
  Serial.println("done.");
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
}

void ejectState(){
  digitalWrite(REDPIN, HIGH);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, LOW);
}

void errorState(){
  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, HIGH);
}

void writeState(){
  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, HIGH);
}
void okState(){
  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, HIGH);
  digitalWrite(BLUEPIN, LOW);
}

void changeSD(){
  write2sd = !write2sd;
  if(!write2sd){
    ejectState();
  } else{
    writeState();
  }
}
