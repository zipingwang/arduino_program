int pinCookingPlateOnOff = 2;
int pinCookingPlateModeSwitch=3;
int pinCookingPlateTempraturePlus=4;
int pinCookingPlateTemperatureMin=5;

// the setup routine runs once when you press reset:
void setup() { 
}

// the loop routine runs over and over again forever:
void loop() {
  delay(1000);
  //turn on
  pinMode(pinCookingPlateOnOff, OUTPUT); 
  digitalWrite(pinCookingPlateOnOff, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(200); 
  pinMode(pinCookingPlateOnOff, INPUT);
  delay(2000);
  
  
  //switch temperature mode
  pinMode(pinCookingPlateModeSwitch, OUTPUT); 
  digitalWrite(pinCookingPlateModeSwitch, LOW);
  delay(200);
  pinMode(pinCookingPlateModeSwitch, INPUT);
  delay(1000);
   
   //raise temprature
   pinMode(pinCookingPlateTempraturePlus, OUTPUT); 
   digitalWrite(pinCookingPlateTempraturePlus, LOW);
   delay(200);
   pinMode(pinCookingPlateTempraturePlus, INPUT);
   delay(5000);
  
 
  //lower temperature
  for(int icount=0; icount<4; icount++)
  {
     pinMode(pinCookingPlateTemperatureMin, OUTPUT); 
     digitalWrite(pinCookingPlateTemperatureMin, LOW);
     delay(200);
     pinMode(pinCookingPlateTemperatureMin, INPUT);
     delay(3000);
  }
  
   //turn off
   pinMode(pinCookingPlateOnOff, OUTPUT); 
   digitalWrite(pinCookingPlateOnOff, LOW); 
   delay(200);   
   pinMode(pinCookingPlateOnOff, INPUT);
   delay(5000);
}
