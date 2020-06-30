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
#define MAX_AUTO_WATER_TRY 2
#define MAX_SENSORS 100
#define BUTTON_PIN 13
#define debug false


uint8_t mois_analog_pins[] = {A0, A1};
uint8_t mois_digital_pins[] = {9, 10};
int mois_values[MAX_SENSORS];
int mois_state[MAX_SENSORS];
int number_of_sensors = *(&mois_analog_pins + 1) - mois_analog_pins;
int watering_try = 0;
int watering_delay = 0;
int button_state = 0;
int auto_water_try = 1;
unsigned long auto_water_delay = 300;
bool moise_watering = false;
bool auto_watering = false;
unsigned long last_time_watering = 0;
int last_time_watering_amount = 0;
LiquidCrystal  lcd(RsPIN, EnPIN, D4PIN, D5PIN, D6PIN, D7PIN);

void setup(){
  Serial.begin(9600);
  pinMode(RELAYPIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  for(int i = 0; i < number_of_sensors; i++){
    pinMode(mois_digital_pins[i], INPUT);
  }
  lcd.begin(16,2);
  lcd.clear();
  // attachInterrupt(digitalPinToInterrupt(BUTTON_PI), , CHANGE);
}


void loop() {

  button_state = digitalRead(BUTTON_PIN);
  if (button_state == HIGH) {
    switch (auto_water_delay){
      case 300:
      if(auto_water_try < MAX_AUTO_WATER_TRY){
        auto_water_try += 1;
      }else{
        auto_water_try = 1;
        auto_water_delay = 600;
      }
      break;

      case 600:
      if(auto_water_try < MAX_AUTO_WATER_TRY){
        auto_water_try += 1;
      }else{
        auto_water_try = 1;
        auto_water_delay = 1800;
      }
      break;

      case 1800:
      if(auto_water_try < MAX_AUTO_WATER_TRY){
        auto_water_try += 1;
      }else{
        auto_water_try = 1;
        auto_water_delay = 3600;
      }
      break;

      case 3600:
      if(auto_water_try < MAX_AUTO_WATER_TRY){
        auto_water_try += 1;
      }else{
        auto_water_try = 1;
        auto_water_delay = 300;
      }
      break;
    }
  }

  for(int i = 0; i < number_of_sensors; i++){
    mois_values[i] = analogRead(mois_analog_pins[i]);
    mois_state[i] = digitalRead(mois_digital_pins[i]);
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
    moise_watering = true;
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
    moise_watering = false;
    watering_delay = max(watering_delay-1, 0);
  }

  if((last_time_watering + auto_water_delay * (unsigned long)1000) < millis()){
    if(last_time_watering_amount < auto_water_try){
      auto_watering = true;
      last_time_watering_amount += 1;
    }else{
      auto_watering = false;
      last_time_watering_amount = 0;
      last_time_watering = millis();
    }
  }else{
    auto_watering = false;
  }

  if(debug){
    Serial.print(number_of_sensors);
    Serial.print("\t");
    Serial.print(auto_water_delay);
    Serial.print("\n");
    Serial.print(moise_watering);
    Serial.print("\t");
    Serial.print(auto_watering);
    Serial.print("\n");
    Serial.print(last_time_watering);
    Serial.print("\t");
    Serial.print(millis());
    Serial.print("\n");
    Serial.print(last_time_watering_amount);
    Serial.print("\n");
  }

  lcd.setCursor(0, 0);
  for(int i = 0; i < number_of_sensors; i++){
    lcd.print(mois_values[i]);
    lcd.print("-");
  }
  lcd.setCursor(0, 1);
  lcd.print(auto_water_delay/60);
  lcd.print(" min ");
  lcd.print(auto_water_try);
  lcd.print(" try ");

  if(moise_watering || auto_watering){
    digitalWrite(RELAYPIN, LOW);
  }else{
    digitalWrite(RELAYPIN,HIGH);
  }
  delay(1000);
}