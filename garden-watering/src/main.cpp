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
#define LOWWATERMODE false
#define RELAYPIN 8
#define MAX_SENSORS 100
#define BUTTON_PIN 13
#define debug false
#define MS_0 0UL
#define MS_1MIN 60000UL
#define MS_10MIN 600000UL
#define MS_30MIN 1800000UL
#define MS_1H 3600000UL
#define MS_2H 7200000UL
#define MS_6H 21600000UL
#define MS_12H 43200000UL
#define RELAY_DELAY_MS 1000UL
#define MAX_MOIS_PROPER_VALUE 750

uint8_t mois_analog_pins[] = {A0, A1};
uint8_t mois_digital_pins[] = {9, 10};
int mois_values[MAX_SENSORS];
int mois_state[MAX_SENSORS];
int number_of_sensors = *(&mois_analog_pins + 1) - mois_analog_pins;
int button_state = 0;
unsigned long moise_watering_delay = MS_10MIN;
unsigned long auto_water_delay = MS_10MIN;
bool moise_watering = false;
bool auto_watering = false;
unsigned long last_time_auto_watering = 0;
unsigned long last_time_moise_watering = 0;
unsigned long time_now = 0;
LiquidCrystal lcd(RsPIN, EnPIN, D4PIN, D5PIN, D6PIN, D7PIN);

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

void show_time(unsigned long t, LiquidCrystal lcd){
  if(t >= MS_1H){
    lcd.print((int)(t/MS_1H));
    if(t % MS_1H > 0){
      lcd.print(":");
      lcd.print((t % MS_1H)/MS_1MIN);
    }
    lcd.print(" H ");
  }else{
    lcd.print(t/MS_1MIN);
    lcd.print(" M ");
  }
}
void loop() {
  time_now = millis();
  lcd.clear();
  button_state = digitalRead(BUTTON_PIN);
  if (button_state == HIGH) {
    switch (auto_water_delay){
      case MS_10MIN:
      auto_water_delay = MS_30MIN;
      break;

      case MS_30MIN:
      auto_water_delay = MS_1H;
      break;

      case MS_1H:
      auto_water_delay = MS_2H;
      break;

      case MS_2H:
      auto_water_delay = MS_6H;
      break;

      case MS_6H:
      auto_water_delay = MS_12H;
      break;

      case MS_12H:
      auto_water_delay = MS_0;
      break;

      case MS_0:
      auto_water_delay = MS_10MIN;
      break;

    }
  }

  if(LOWWATERMODE){
    moise_watering_delay = (unsigned long)(2UL * auto_water_delay / 3UL);
  }else{
    moise_watering_delay = (unsigned long)(auto_water_delay / 3UL);
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
    if(mois_state[i] == HIGH and mois_values[i] < MAX_MOIS_PROPER_VALUE){
      is_dry = true;
    }
  }

  if(auto_water_delay < time_now - last_time_auto_watering){
    auto_watering = true;
    last_time_auto_watering = time_now;
  }else{
    auto_watering = false;
  }
  
  if(moise_watering_delay < time_now - last_time_moise_watering){
    last_time_moise_watering = time_now;
    if(is_dry && !is_one_wet){
      moise_watering = true;
    }
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
    Serial.print(time_now);
    Serial.print("\n");
  }

  lcd.setCursor(0, 0);
  for(int i = 0; i < number_of_sensors; i++){
    lcd.print(mois_values[i]);
    lcd.print("-");
  }
  lcd.setCursor(0, 1);
  show_time(auto_water_delay, lcd);
  show_time(moise_watering_delay, lcd);
  lcd.print((last_time_auto_watering - time_now + auto_water_delay)/MS_1MIN);
  lcd.print(" ");
  lcd.print((last_time_moise_watering - time_now + moise_watering_delay)/MS_1MIN);
  
  if(moise_watering || auto_watering){
    digitalWrite(RELAYPIN, LOW);
    delay(RELAY_DELAY_MS);
    digitalWrite(RELAYPIN, HIGH);
  }else{
    digitalWrite(RELAYPIN, HIGH);
  }
  delay(1000);
}