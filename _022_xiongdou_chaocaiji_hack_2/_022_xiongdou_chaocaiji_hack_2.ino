/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin pinLed. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
int pinLed=7;
int pinYule=A0;
int pinHeatBottom=10;
int pinMotor=9;
int pinHeatTop=8;



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin pinLed as an output.
  pinMode(pinLed, OUTPUT);
  pinMode(pinYule, INPUT_PULLUP);
  pinMode(pinHeatBottom, OUTPUT);
  pinMode(pinHeatTop, OUTPUT);
  pinMode(pinMotor, OUTPUT);
  
  digitalWrite(pinHeatBottom, LOW);
  digitalWrite(pinHeatTop, LOW);
  digitalWrite(pinMotor, LOW);
  
  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
  
   Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
Serial.println(digitalRead(pinYule));
Serial.println(analogRead(pinYule));
delay(1000);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    //inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == 'a') {
       digitalWrite(pinHeatBottom, HIGH);  
       digitalWrite(pinHeatTop, HIGH);  
       
       while(digitalRead(pinYule)==LOW)
       {
         Serial.println(analogRead(pinYule));
         delay(1000);
         //analogWrite(pinHeatBottom, 125);  // the duty cycle: between 0 (always off) and 255 (always on).
         //analogWrite(pinHeatTop, 125);  
          
       }
       digitalWrite(pinHeatBottom, LOW);  
       digitalWrite(pinHeatTop, LOW); 
       
       digitalWrite(pinLed, HIGH);
       delay(1000);
    }
    else if(inChar=='b')
    {
         digitalWrite(pinHeatBottom, HIGH);  
         digitalWrite(pinHeatTop, HIGH);  
    }
    else  if (inChar == 'c') {
        digitalWrite(pinMotor, HIGH);
        delay(8000);
        digitalWrite(pinMotor, LOW);
    }
     else  if (inChar == 'd') {
       digitalWrite(pinHeatBottom, LOW);  
       digitalWrite(pinHeatTop, LOW);   
    }
      else if(inChar=='e')
    {
        analogWrite(pinHeatBottom, 160); 
        analogWrite(pinHeatTop, 160);  
    }
  }
}
