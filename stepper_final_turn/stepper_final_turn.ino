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
Stepper myStepper2(stepsPerRevolution, 8,10,9,11);    

boolean flag=false;

//use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
int turnPin=3;

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(400);
  myStepper2.setSpeed(400);
  //use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
  // pinMode(turnPin, INPUT_PULLUP);
  
  // initialize the serial port:
  Serial.begin(9600);
 //   for(int icount=0; icount<stepsPerRevolution*320; icount+=32)
 //   {
 //     if(digitalRead(turnPin)==LOW)
 //       break;
 //     myStepper.step(32);
 //   }
 //   myStepper.step(-stepsPerRevolution*18); //64*32 is 360 degree.
 //   delay(2000);
}

void loop() {
 
  // step one revolution  in one direction:
   Serial.println("clockwise");
   //myStepper.step(64);
    // myStepper.step(stepsPerRevolution*18); //64*32 is 360 degree.
   // for(int icount=0; icount<stepsPerRevolution*30; icount+=32)
   // {
   //   if(digitalRead(turnPin)==LOW)
   //     break;
   //   myStepper.step(-32);
   // }
   
   myStepper.step(stepsPerRevolution*-17); //64*32 is 360 degree.
   delay(1000);
   myStepper.step(stepsPerRevolution*17); //64*32 is 360 degree.
   delay(1000); 
     
   myStepper2.step(stepsPerRevolution*-17); //64*32 is 360 degree.
   delay(1000);
   myStepper2.step(stepsPerRevolution*17); //64*32 is 360 degree.
   delay(1000); 
}
