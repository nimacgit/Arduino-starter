#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>
#include "LiquidCrystal.h"

/*
SD card: 5V
MOSI - pin 11
MISO - pin 12
CLK - pin 13
CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

DHT: 5V

moisSensor: 3.3V

LCD: 5V
K - GND
A - 5V
Rw - GND
V0 - potentio
VLD - 5V
VSS - GND
*/

#define DHT11_PIN 3
#define MOIS_PIN A1
#define MOIS2_PIN A2
#define MOIS3_PIN A3
#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 7
#define INTERRUPTPIN 2
#define RsPIN 10
#define EnPIN 9
#define D4PIN 8
#define D5PIN 7
#define D6PIN 6
#define D7PIN 5
#define DEBUGMOD false
#define EJECTSTATE "EJ"
#define ERRORSTATE "ER"
#define OKSTATE "OK"

LiquidCrystal  lcd(RsPIN, EnPIN, D4PIN, D5PIN, D6PIN, D7PIN);

File myFile;
DHT DHT_sens(DHT11_PIN, DHT11);

int savePeriod = 5000;
int LCDPeriod = 1000;
unsigned long saveTime = 0;
unsigned long LCDTime = 0;
float h, t;
bool write2sd = false;
int moisValue;
int mois2Value;
int mois3Value;
char fileName[] = "SG";
int lastFileNum = -1;
bool interuptPinVal;
char* state;

void writeToFile();
void changeSD();
void startSD();
void stopSD();
void printState();
void ejectState();
void errorState();
void okState();
void writeState();
void displaySensor();
void initSD();
void initLCD();
void initLED();
void initDHT();
void initInterrupt();
void readSensors();
void setPins();

void setup()
{
  initDHT();
  // initInterrupt();
  initLCD();
  Serial.begin(9600);
  initSD();
  LCDTime = saveTime = millis();
}

void loop() {
  readSensors();
  setPins();
  if((millis() > LCDTime + LCDPeriod)){
    LCDTime = millis();
    displaySensor();
    if(!write2sd){
      ejectState();
    }
  }
  if((millis() > saveTime + savePeriod) && write2sd){
      saveTime = millis();
      writeToFile();
  }
}











void openfile(){
  int num = 1;
  char strName[16];
  // sprintf(strNum, "%d", num);
  // strcpy(strName, fileName);
  // strcat(strName, strNum);
  sprintf(strName, "%s%d", fileName, num);
  while(SD.exists(strName) && (num < lastFileNum || lastFileNum == -1)){
    num++;
    sprintf(strName, "%s%d", fileName, num);
  }
  lastFileNum = num;
  myFile = SD.open(strName, FILE_WRITE);
}

void writeToFile(){
  openfile();
  if (myFile) {
    writeState();
    myFile.println(saveTime);
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
  }
}

void printSensor(){
  Serial.println(saveTime);
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
  Serial.println("Writing to TermoHu.txt...");
}


void displaySensor(){
  lcd.setCursor(0, 0);
  lcd.print("");
  lcd.print(t);
  lcd.print("C ");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("M:");
  lcd.print(moisValue);
  lcd.print(" ");
  lcd.print(mois2Value);
  lcd.print(" ");
  lcd.print(mois3Value);
}

void LCDEjectState();
void LCDErrorState();
void LCDOkState();
void LCDWriteState();
void LEDEjectState();
void LEDErrorState();
void LEDOkState();
void LEDWriteState();

void ejectState(){
  state = EJECTSTATE;
  if(DEBUGMOD){
    Serial.println("Eject mode");
  }
  LCDEjectState();
  // LEDEjectState();
}
void errorState(){
  state = ERRORSTATE;
  if(DEBUGMOD){
    Serial.println("error opening file");
  }
  LCDErrorState();
  // LEDErrorState();
}
void okState(){
  state = OKSTATE;
  if(DEBUGMOD){
    Serial.println("Okaye");
  }
  LCDOkState();
  // LEDOkState();
}
void writeState(){
  if(DEBUGMOD){
    Serial.println("writing");
    printSensor();
  }
  LCDWriteState();
  // LEDWriteState();
}

void LEDEjectState(){
  digitalWrite(REDPIN, HIGH);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, LOW);
}

void LEDErrorState(){
  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, HIGH);
}

void LEDWriteState(){
  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, HIGH);
}
void LEDOkState(){
  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, HIGH);
  digitalWrite(BLUEPIN, LOW);
}

void LCDEjectState(){
  lcd.setCursor(13,0);
  lcd.print(" EJ");
}
void LCDErrorState(){
  lcd.setCursor(13,0);
  lcd.print(" ER");
}
void LCDOkState(){
  lcd.setCursor(13,0);
  lcd.print(" OK");
}
void LCDWriteState(){
  lcd.setCursor(13,0);
  lcd.print(" WR");
}

void startSD(){
  write2sd = true;
}

void stopSD(){
  write2sd = false;
}

void changeSD(){
  write2sd = !write2sd;
  if(!write2sd){
    ejectState();
  } else{
    writeState();
  }
}

void initLED(){
  pinMode(GREENPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  errorState();
}
void initDHT(){
  DHT_sens.begin();
}

void initInterrupt(){
  pinMode(INTERRUPTPIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), changeSD, FALLING);
  // attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), stopSD, FALLING);
  // attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), startSD, RISING);
}

void initSD(){
  if(DEBUGMOD){
    Serial.print("Initializing SD card...");
  }
  while(!SD.begin(4)){
    if(DEBUGMOD){
      Serial.println("initialization failed!");
    }
    delay(1000);
  }
  if(DEBUGMOD){
    Serial.println("initialization done.");
  }
}

void initLCD(){
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Hello 2 SG");
}

void readSensors(){
  h = DHT_sens.readHumidity();
  t = DHT_sens.readTemperature();
  moisValue = analogRead(MOIS_PIN);
  mois2Value = analogRead(MOIS2_PIN);
  mois3Value = analogRead(MOIS3_PIN);
}

void setPins(){
  if(digitalRead(INTERRUPTPIN) == HIGH){
    write2sd = true;
  } else{
    write2sd = false;
  }
}
