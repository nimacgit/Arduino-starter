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
// #define NORMALWATERINGTRY 5
// #define LOWWATERINGTRY 3
#define NORMAL_WATERING_DELAY 300
#define LOW_WATERING_DELAY 600
#define LOWWATERMODE false
#define RELAYPIN 8
#define MAX_SENSORS 100
#define BUTTON_PIN 13
#define debug false


uint8_t mois_analog_pins[] = {A0, A1};
uint8_t mois_digital_pins[] = {9, 10};
int mois_values[MAX_SENSORS];
int mois_state[MAX_SENSORS];
int number_of_sensors = *(&mois_analog_pins + 1) - mois_analog_pins;
int button_state = 0;
unsigned long moise_watering_delay = 200;
unsigned long auto_water_delay = 600;
bool moise_watering = false;
bool auto_watering = false;
unsigned long last_time_auto_watering = 0;
unsigned long last_time_moise_watering = 0;
LiquidCrystal lcd(RsPIN, EnPIN, D4PIN, D5PIN, D6PIN, D7PIN);
int normal_watering_delay = NORMAL_WATERING_DELAY;
int low_watering_delay = LOW_WATERING_DELAY;

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
      case 600:
      auto_water_delay = 1800;
      break;

      case 1800:
      auto_water_delay = 3600;
      break;

      case 3600:
      auto_water_delay = 7200;
      break;

      case 7200:
      auto_water_delay = 21600;
      break;

      case 21600:
      auto_water_delay = 43200;
      break;

      case 43200:
      auto_water_delay = 600;
      break;
    }
  }
  if(LOWWATERMODE){
    moise_watering_delay = (unsigned long)(2 * auto_water_delay / 3);
  }else{
    moise_watering_delay = (unsigned long)(auto_water_delay / 3);
  }

  if(last_time_auto_watering + auto_water_delay * (unsigned long)1000 < millis()){
    auto_watering = true;
    last_time_auto_watering = millis();
  }else{
    auto_watering = false;
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

  
  if(!auto_watering && is_dry && !is_one_wet && (last_time_moise_watering + moise_watering_delay * (unsigned long)1000 < millis())){
    moise_watering = true;
    last_time_moise_watering = millis();
  }
  else{
    moise_watering = false;
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
    Serial.print(last_time_auto_watering);
    Serial.print("\t");
    Serial.print(last_time_moise_watering);
    Serial.print("\t");
    Serial.print(millis());
    Serial.print("\n");
  }

  lcd.setCursor(0, 0);
  for(int i = 0; i < number_of_sensors; i++){
    lcd.print(mois_values[i]);
    lcd.print("-");
  }
  lcd.setCursor(0, 1);
  if(auto_water_delay >= 3600){
    lcd.print((int)(auto_water_delay/3600));
    if(auto_water_delay % 3600 > 0){
      lcd.print(":");
      lcd.print((auto_water_delay % 3600)/60);
    }
    lcd.print(" H ");
  }else{
    lcd.print(auto_water_delay/60);
    lcd.print(" M ");
  }
  lcd.print((int)((last_time_auto_watering + auto_water_delay * (unsigned long)1000 - millis())/60000));
  lcd.print(" ");
  lcd.print((int)((last_time_moise_watering + moise_watering_delay * (unsigned long)1000 - millis())/60000));
  lcd.print(" ");
  
  if(moise_watering || auto_watering){
    digitalWrite(RELAYPIN, LOW);
  }else{
    digitalWrite(RELAYPIN,HIGH);
  }
  delay(1000);
}