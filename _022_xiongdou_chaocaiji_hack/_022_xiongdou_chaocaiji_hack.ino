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
int pin0=A0;
int pin1=A1;
int pin2=A2;
int pin3=A3;
int pin4=8;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin pinLed as an output.
  pinMode(pinLed, OUTPUT);
   Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  //digitalWrite(pinLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);              // wait for a second
  //digitalWrite(pinLed, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);           l   // wait for a second
  
  /*
    Serial.println("pin0");
     Serial.println(analogRead(pin0));
     
    Serial.println("pin1");
     Serial.println(analogRead(pin1));
     
    Serial.println("pin2");
     Serial.println(analogRead(pin2));
     
    Serial.println("pin3");
     Serial.println(analogRead(pin3));
     
    Serial.println("pin4");
     Serial.println(analogRead(pin4));
     
     delay(2000);
     */
     
  if(digitalRead(pin0)==HIGH)
  {
    Serial.println("pin0 high");
     Serial.println(analogRead(pin0));
    digitalWrite(pinLed, HIGH);
    delay(2000);
  }
  if(digitalRead(pin1)==HIGH)
  {
    Serial.println("pin1 high");
    Serial.println(analogRead(pin1));
    digitalWrite(pinLed, HIGH);
    delay(2000);
  }
  if(digitalRead(pin2)==HIGH)
  {
    Serial.println("pin2 high");
    Serial.println(analogRead(pin2));
    digitalWrite(pinLed, HIGH);
    delay(2000);
  }
  
  /*
  if(digitalRead(pin3)==HIGH)
  {
    Serial.println("pin3 high");
    //Serial.println(analogRead(pin3));
    digitalWrite(pinLed, HIGH);
    delay(2000);
  }
  */
  /*
   if(digitalRead(pin4)==LOW)
  {
    Serial.println("pin4 LOW");
    Serial.println(analogRead(pin4));
    digitalWrite(pinLed, HIGH);
    delay(2000);
  }
   if(digitalRead(pin4)==HIGH)
  {
    Serial.println("pin4 HIGH");
    Serial.println(analogRead(pin4));
    digitalWrite(pinLed, HIGH);
    delay(2000);
  }
  */
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
      //stringComplete = true;
      //reset
      pinMode(pin4, OUTPUT);
      digitalWrite(pin4, HIGH);
      delay(200);
      digitalWrite(pin4, LOW);
      //pinMode(pin4, INPUT);
      //digitalWrite(pin4, HIGH);
    }
    else if(inChar=='b')
    {
 
    }
    else  if (inChar == 'c') {
      //stringComplete = true;
      pinMode(pin3, OUTPUT);
      analogWrite(pin3, 1023);
      //delay(2000);
      //analogWrite(pin3, LOW);
      //pinMode(pin4, INPUT);
      //digitalWrite(pin4, HIGH);
    }
  }
}
