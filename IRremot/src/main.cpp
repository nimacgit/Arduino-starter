#include <Arduino.h>
#include <IRremote.h>  // use the IRRemote.h
const int irReceiverPin = 8;  //the SIG of receiver module attach to pin2
IRrecv irrecv(irReceiverPin); //Creates a variable of type IRrecv
decode_results results;    // define resultsas 
IRsend irsend;
                                
void setup()
{
  Serial.begin(9600);    //initialize serial,baudrate is 9600
  irrecv.enableIRIn();   // enable ir receiver module
  irrecv.blink13(true);
}

void loop() 
{
  for(int i = 0; i < 1000; i++){
    irsend.sendLG(0xFF02FD, 12);
    delay(i);
    if (irrecv.decode(&results)) //if the ir receiver module receiver data
    {  
      Serial.print(i);
      Serial.print("irCode: ");    //print "irCode: "        
      Serial.print(results.value, HEX); //print the value in hexdecimal 
      Serial.print(",  bits: ");  //print" , bits: "         
      Serial.println(results.bits); //print the bits
      irrecv.resume();    // Receive the next value 
    }
    // delay(2000);
    delay(600); //delay 600ms
  }
}