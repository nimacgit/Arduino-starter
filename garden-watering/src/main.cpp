#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
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
#define DRYMOISMIN 400
#define DRYMOISMAX 600
#define WETMOIS 300
#define NORMALWATERINGTRY 5
#define NORMALWATERINGDELAY 300
#define LOWWATERINGTRY 3
#define LOWWATERINGDELAY 600
#define LOWWATERMODE false
#define RELAYPIN 8
#define MAX_SENSORS 100

uint8_t mois_analog_pins[] = {A0, A1};
uint8_t mois_digital_pins[] = {10, 11};
int mois_values[MAX_SENSORS];
int mois_state[MAX_SENSORS];
int number_of_sensors = *(&mois_analog_pins + 1) - mois_analog_pins;
int watering_try = 0;
int watering_delay = 0;
LiquidCrystal  lcd(RsPIN, EnPIN, D4PIN, D5PIN, D6PIN, D7PIN);

void setup(){
  Serial.begin(9600);
  pinMode(RELAYPIN, OUTPUT);
  for(int i = 0; i < number_of_sensors; i++){
    pinMode(mois_digital_pins[i], INPUT);
  }
  lcd.begin(16,2);
  lcd.clear();
}

void loop() {
  Serial.print(number_of_sensors);
  Serial.print("\n");
  for(int i = 0; i < number_of_sensors; i++){
    mois_values[i] = analogRead(mois_analog_pins[i]);
    mois_state[i] = digitalRead(mois_digital_pins[i]);
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
    // if(mois_values[i] > DRYMOISMIN && mois_values[i] < DRYMOISMAX){
    //   is_dry = true;
    // }
    // if(mois_values[i] < WETMOIS){
    //   is_one_wet = true;
    // }
    if(mois_state[i] == LOW){
      is_one_wet = true;
    }
    if(mois_state[i] == HIGH and mois_values[i] < 650){
      is_dry = true;
    }
  }
  if(is_dry && !is_one_wet && watering_delay == 0){
    watering_try += 1;
    digitalWrite(RELAYPIN, LOW);
    if(LOWWATERMODE){
      if(watering_try >= LOWWATERINGTRY){
        watering_delay = LOWWATERINGDELAY;
        watering_try = 0;
      }
    }else{
      if(watering_try >= NORMALWATERINGTRY){
        watering_delay = NORMALWATERINGDELAY;
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