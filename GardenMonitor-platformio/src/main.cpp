#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>
#include <LiquidCrystal.h>

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
#define interruptPin 2
#define RsPIN 10
#define EnPIN 9
#define D4PIN 8
#define D5PIN 7
#define D6PIN 6
#define D7PIN 5

LiquidCrystal  lcd(RsPIN, EnPIN, DPIN, D5PIN, D6PIN, D7PIN);

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
void displayState();
void initSD();
void initLCD();
void initLED();
void initDHT();
void initInterrupt();

void setup()
{
  initLED();
  initDHT();
  initInterrupt();
  initLCD();
  Serial.begin(9600);
  initSD();
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

void displayState(){
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

void initLED(){
  pinMode(GREENPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  errorState();
}
void initDHT(){
  DHT_sens.begin();
}

void initInterrupt(){
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeSD, FALLING);
}

void initSD(){
  Serial.print("Initializing SD card...");
  while(!SD.begin(4)){
    Serial.println("initialization failed!");
    delay(1000);
  }
  Serial.println("initialization done.");
}

void initLCD(){
  lcd.begin(16, 2);
  lcd.clear();
}