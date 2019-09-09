#include "DHT.h"

float h, t;
#define DHT11_PIN 3
DHT DHT_sens(DHT11_PIN, DHT11);           //datapin sensor to pin 10 Arduino



void setup()
{
   DHT_sens.begin();
   Serial.begin (9600);
}




void loop(){


  
  // ================== read from buffer and display =========================    
  
  h = DHT_sens.readHumidity();
  t = DHT_sens.readTemperature();
  
  delay (100);                       // pause a second
  Serial.print ("Humidity: "); 
  Serial.print (h,0);                  // zero decimal
  Serial.print (" %\t");
  Serial.print ("Temperature: "); 
  Serial.print (t,1);                  // one decimal
  Serial.println (" *C");
  delay (1000);                       // pause a second
}
