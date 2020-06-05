#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal.h>

#define RELAYPIN 8
#define MAX_SENSORS 100
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

uint8_t mois_pins[] = {A0, A1, A2};
int mois_values[MAX_SENSORS];
int number_of_sensors = *(&mois_pins + 1) - mois_pins;
int watering_try = 0;
int watering_delay = 0;
LiquidCrystal  lcd(RsPIN, EnPIN, D4PIN, D5PIN, D6PIN, D7PIN);

void setup(){
  Serial.begin(9600);
  pinMode(RELAYPIN, OUTPUT);
  lcd.begin(16,2);
  lcd.clear();
}

void loop() {
  for(int i = 0; i < number_of_sensors; i++){
    mois_values[i] = analogRead(mois_pins[i]);
  }
  lcd.setCursor(0, 0);
  for(int i = 0; i < number_of_sensors; i++){
    lcd.print(mois_values[i]);
    lcd.print("-");
    Serial.print(mois_values[i]);
    Serial.print("\n");
  }


  bool is_dry = false;
  bool is_one_wet = false;
  for(int i = 0; i < number_of_sensors; i++){
    if(mois_values[i] > DRYMOISMIN && mois_values[i] < DRYMOISMAX){
      is_dry = true;
    }
    if(mois_values[i] < 100){
      is_one_wet = true;
    }
  }
  if(is_dry && !is_one_wet){
    if(watering_delay > 0){
      watering_delay -= 1;
    }else{
      watering_try += 1;
      digitalWrite(RELAYPIN, LOW);
      if(watering_try >= 10){
        watering_delay = 300;
        watering_try = 0;
      }
    }
  }
  else{
    digitalWrite(RELAYPIN,HIGH);
    watering_delay = max(watering_delay-1, 0);
  }
  delay(1000);

}