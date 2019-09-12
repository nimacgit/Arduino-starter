void calcHall(){
  pinMode(PINHALL, INPUT);
  Serial.begin(9600);   // Initiate a serial communication
  
  long measure = 0;
  for(int i = 0; i < 10; i++){
      int value = 
      measure += analogRead(PINHALL);
  }
  measure /= 10;  
  //voltage in mV
  float outputV = measure * 5000.0 / 1023;
  Serial.print("Output Voltaje = ");
  Serial.print(outputV);
  Serial.print(" mV   ");
  
  //flux density
  float magneticFlux =  outputV * 53.33 - 133.3;
  Serial.print("Magnetic Flux Density = ");
  Serial.print(magneticFlux);
  Serial.print(" mT");  
}