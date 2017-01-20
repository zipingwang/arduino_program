/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int pinCookingPlateOnOff = 42;
int pinCookingPlateMode=44;
int pinCookingPlateTPlus=46;
int pinCookingPlateTMin=48;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(pinCookingPlateOnOff, OUTPUT);  
  pinMode(pinCookingPlateMode, OUTPUT);  
  pinMode(pinCookingPlateTPlus, OUTPUT);  
  pinMode(pinCookingPlateTMin, OUTPUT);  
  
  digitalWrite(pinCookingPlateOnOff, HIGH);
  digitalWrite(pinCookingPlateMode, HIGH);
  digitalWrite(pinCookingPlateTPlus, HIGH);
  digitalWrite(pinCookingPlateTMin, HIGH);
   delay(2000);
}

// the loop routine runs over and over again forever:
void loop() {
  
  //turn on cookingplate. relay is default open
  ConnectPin(pinCookingPlateOnOff);
  
  ConnectPin(pinCookingPlateMode);        // wait for a second
  
  
  ConnectPin(pinCookingPlateTPlus); 
  ConnectPin(pinCookingPlateTPlus); 
  ConnectPin(pinCookingPlateTPlus); 
  
  ConnectPin(pinCookingPlateTMin); 
  
  //cooking
  delay(5000);
  ConnectPin(pinCookingPlateOnOff);
  delay(2000);
 
}

void ConnectPin(int pin)
{
   digitalWrite(pin, LOW);   // turn the LED on (HIGH is the voltage level)
   delay(200);               // wait for a second
   digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
   delay(200);     
}
