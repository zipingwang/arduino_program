/* 
 Stepper Motor Control - one revolution
 
 This program drives a unipolar or bipolar stepper motor. 
 The motor is attached to digital pins 8 - 11 of the Arduino.
 
 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.  
 
  
 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe
 
 */

//http://forum.pjrc.com/threads/23763-28BYJ-48-stepper-motor-works

#include <Stepper.h>

const int stepsPerRevolution = 64;  // change this to fit the number of steps per revolution
//const int stepsPerRevolution = 500;  // change this to fit the number of steps per revolution
                                     // for your motor
//const int stepsPerRevolution = 4096;

// initialize the stepper library on pins 8 through 11:
//Stepper myStepper(stepsPerRevolution, 8,9,10,11);   
//ziping note pin 1,2,3,4 connect to 8,9,10,11, but when init 8, 10, 9, 11
Stepper myStepper(stepsPerRevolution, 4,6,5,7);            

boolean flag=false;

//use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
int turnPin=12;
int motorStopPin=3;



int motorPin1 = 8;   // A+
int motorPin2 =10;   // B+
int motorPin3 =9;    // A-
int motorPin4 = 11;  // B-                       

int motorSpeed =2000; //for 4 steps: minimun 1500, how smaller the number,how faster. 16385 is the maximun. then restart, e.g 18385=2000
 //for 8 steps: minimun 600, how smaller the number,how faster. 16385 is the maximun. then restart, e.g 18385=2000. 5000 could be the best
 
int count = 0;          // count of steps made
int countsperrev =50;// how many rotation, how long distance it will move.

int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
int steps=8;

//int steps=4;
//int lookup[4] = {B01000, B00100, B00010, B00001};

int turnDirection=-1;
int halfTurnStepCount=17; //one step is 17

void setup() {
  
  //declare the motor pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  
   
  // set the speed at 60 rpm:
  myStepper.setSpeed(400);
   //use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
   pinMode(turnPin, INPUT_PULLUP);
   pinMode(motorStopPin, INPUT_PULLUP);
   
  // initialize the serial port:
  Serial.begin(9600);
    for(int icount=0; icount<stepsPerRevolution*320; icount+=32)
    {
      if(digitalRead(turnPin)==LOW)
        break;
      myStepper.step(32);
    }
    delay(2000);
    /*
    int icountTemp=0;
     myStepper.step(-stepsPerRevolution*10);
    for(int icount=0; icount<stepsPerRevolution*200; icount+=stepsPerRevolution)
    {
      myStepper.step(-stepsPerRevolution);
      icountTemp++;
      if(digitalRead(turnPin)==LOW)
        break;      
    }
    Serial.println("icountTemp");
     Serial.println(icountTemp);
    halfTurnStepCount=icountTemp/2 +10/2 +1;
    */
    halfTurnStepCount=23;
    /*
     myStepper.step(320);
       for(int icount=0; icount<stepsPerRevolution*320; icount+=32)
    {
      if(digitalRead(turnPin)==LOW)
        break;
      myStepper.step(32);
    }
    */
    moveForward();
    //myStepper.step(-stepsPerRevolution*18); //64*32 is 360 degree.
    delay(2000);
}

void loop() {
 
  moveBack();
  halfTurn();
  moveForward();
   delay(2000);  
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)
void anticlockwise()
{
  for(int i = 0; i < steps; i++)
  // for(int i = 0; i < 4; i++)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void clockwise()
{
  for(int i = steps-1; i >= 0; i--)
  // for(int i = 3; i >= 0; i--)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutput(int out)
{
  digitalWrite(motorPin1, bitRead(lookup[out], 0));
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));
}

void moveBack()
{
 //move back
   count=0;
  while(count < countsperrev )
  {
    
    clockwise();
    count++;
  }
}


void moveForward()
{
 //move back
   count=0;
  while(count < countsperrev*2 )
  {
    if(digitalRead(motorStopPin)==LOW)
        break;
    anticlockwise();
    count++;
  }
}
void halfTurn()
{  
   Serial.println("halfTurnStepCount");
     Serial.println(halfTurnStepCount);
      myStepper.step(-stepsPerRevolution*halfTurnStepCount); //64*32 is 360 degree.
      delay(500); 
      /*
        myStepper.step(stepsPerRevolution*2); //64*32 is 360 degree.
        delay(500); 
      myStepper.step(-stepsPerRevolution*2); //64*32 is 360 degree.
        delay(500); 
         myStepper.step(stepsPerRevolution*2); //64*32 is 360 degree.
        delay(500); 
      myStepper.step(-stepsPerRevolution*2); //64*32 is 360 degree.
        delay(500); 
      */
      for(int icount=0; icount<stepsPerRevolution*40; icount+=32)
      {
        if(digitalRead(turnPin)==LOW)
          break;
        myStepper.step(32);
      }
     delay(500);
}
