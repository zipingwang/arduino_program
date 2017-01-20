/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int pinElectroValve = 7;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(pinElectroValve, OUTPUT); 
  
  //relay 是低电平有效
  digitalWrite(pinElectroValve, HIGH); 
   delay(2000);
}

// the loop routine runs over and over again forever:
void loop() {
  
  //turn on cookingplate. relay is default open
  ConnectPin(pinElectroValve);
 
}

void ConnectPin(int pin)
{
   digitalWrite(pin, LOW);   // turn the LED on (HIGH is the voltage level)
   delay(5000);               // wait for a second
   digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
   delay(10000);     
}
