#include <Arduino.h>
#include <CapacitiveSensor.h>
#include <SD.h>
#include <SPI.h>

#define melodyPin 3
// int melody[] = { NOTE_E7, NOTE_E7, 0, NOTE_E7, 0, NOTE_C7, NOTE_E7, 0, NOTE_G7, 0, 0, 0, NOTE_G6, 0, 0, 0,
// NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6, 0, NOTE_AS6, NOTE_A6, 0,
// NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0,NOTE_C7, NOTE_D7, NOTE_B6, 0, 0,
// NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6, 0, NOTE_AS6, NOTE_A6, 0,
// NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0,NOTE_C7, NOTE_D7, NOTE_B6, 0, 0 };

#define SOUND_PIN 5

int tempo[] = { 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, };

CapacitiveSensor   cs_2_3 = CapacitiveSensor(2,3);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_2_4 = CapacitiveSensor(2,4);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil


void setup() {
  // cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(57600);
  // pinMode(SOUND_PIN, OUTPUT);
}

void loop() {
  // digitalWrite(SOUND_PIN, LOW);
  long start = millis();
  long total1 =  cs_2_3.capacitiveSensor(30);
  long total2 =  cs_2_4.capacitiveSensor(30);
  if(millis() % 1000 < 10){
    Serial.print(millis() - start);
    Serial.print("\t");
    Serial.print(total1);
    Serial.print("\t");
    Serial.print(total2);
    Serial.print("\n");
  }
  if(total1 > 100){
    // digitalWrite(SOUND_PIN, HIGH);
    for(int i = 0; i < 10; i++){
      tone(SOUND_PIN, 85+5*i, 10);
    }
  }
  if(total2 > 100){
    tone(SOUND_PIN, 524, 100);
  }

  delay(10);
}


