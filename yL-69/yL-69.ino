#include <dht.h>

#define DHT11PIN 4
dht DHT;

byte humidity_sensor_pin = A1;
byte humidity_sensor_vcc = 6;
int sensorPin = 2;

void setup() {
  pinMode(humidity_sensor_vcc, OUTPUT);
  digitalWrite(humidity_sensor_vcc, LOW);
  while (!Serial);
  delay(1000);
  Serial.begin(9600);
}

int read_humidity_sensor() {
  digitalWrite(humidity_sensor_vcc, HIGH);
  delay(500);
  int value = analogRead(humidity_sensor_pin);
  digitalWrite(humidity_sensor_vcc, LOW);
  return 1023 - value;
}


int read_temp(){
  int reading = analogRead(sensorPin);
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
  float temperatureC = (voltage - 0.5) * 100 ;
  return temperatureC;
}

void loop() {
  DHT.read11(DHT11PIN);
//  Serial.print("Humidity Level (0-1023): ");
//  Serial.println(read_humidity_sensor());
//  Serial.print(read_temp());
//  Serial.println(" degrees C");

  Serial.print("Humidity & Temp: ");
  Serial.print(DHT.humidity);
  Serial.print(" ");
  Serial.println(DHT.temperature);
  delay(1000);
}
