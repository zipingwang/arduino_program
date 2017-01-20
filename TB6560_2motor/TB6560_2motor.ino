/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
int pinClkPlus=6;
int pinCW=7;

int pinClkPlus2=4;
int pinCW2=5;
float proportion=0.2;

int motorSpeed=400; //250 is minimum for  sumtor

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(pinClkPlus, OUTPUT);
  pinMode(pinCW, OUTPUT);
  pinMode(pinClkPlus2, OUTPUT);
  pinMode(pinCW2, OUTPUT);
  
  digitalWrite(pinClkPlus, LOW);
  digitalWrite(pinCW, LOW);
  digitalWrite(pinClkPlus2, LOW);
  digitalWrite(pinCW2, LOW);
}

void loop2() {

   digitalWrite(pinClkPlus, HIGH);
   delayMicroseconds(500);
   digitalWrite(pinClkPlus, LOW);
   delayMicroseconds(500);
 
}
// the loop routine runs over and over again forever:
void loop() {
 // Serial.write("h");
  digitalWrite(pinCW, LOW);
  digitalWrite(pinCW2, LOW);
  
  int stepCountM2=0;
  int stepCountM1=1;
  int pinM2=-1;
  float tempValue;
  
 for(int icount=0; icount<1000; icount++)
 {
    tempValue=icount;
    tempValue=tempValue*proportion;
    
    if(tempValue-stepCountM2>=1)
    {
       digitalWrite(pinClkPlus2, HIGH);
        digitalWrite(pinClkPlus, HIGH);  
       delayMicroseconds(motorSpeed);
       digitalWrite(pinClkPlus2, LOW);
       digitalWrite(pinClkPlus, LOW);    
       delayMicroseconds(motorSpeed);
       stepCountM2++;
    }
    else
    {      
      digitalWrite(pinClkPlus, HIGH);  
      delayMicroseconds(motorSpeed);
      digitalWrite(pinClkPlus, LOW);    
      delayMicroseconds(motorSpeed);
    }
 }
 delay(3000);
 /*
 digitalWrite(pinCW, HIGH);
 digitalWrite(pinCW2, HIGH);
 for(int icount=0; icount<1000; icount++)
 {
   digitalWrite(pinClkPlus, HIGH);
   digitalWrite(pinClkPlus2, HIGH);
   delayMicroseconds(1000);
   digitalWrite(pinClkPlus, LOW);
   digitalWrite(pinClkPlus2, LOW);
   delayMicroseconds(1000);   
 }
 delay(3000);
 */
}
