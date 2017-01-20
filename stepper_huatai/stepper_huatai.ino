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

const int stepsPerRevolution = 10;  // change this to fit the number of steps per revolution
//const int stepsPerRevolution = 500;  // change this to fit the number of steps per revolution
                                     // for your motor
//const int stepsPerRevolution = 4096;

// initialize the stepper library on pins 8 through 11:
//Stepper myStepper(stepsPerRevolution, 8,9,10,11);   
//ziping note pin 1,2,3,4 connect to 8,9,10,11, but when init 8, 9, 10, 11
Stepper myStepper(stepsPerRevolution, 8,9,10,11);            

boolean flag=true;
void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(650);
  
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  if(flag)
  {
  // step one revolution  in one direction:
   Serial.println("clockwise");
  myStepper.step(stepsPerRevolution*16); //64*32 is 360 degree.
  delay(2000);
  
   // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution*16);
  delay(2000); 
  //flag=false;
  }
  else
  {
    myStepper.step(-stepsPerRevolution*18);
    delay(2000); 
    myStepper.step(stepsPerRevolution*18); //64*32 is 360 degree.
    delay(2000);
    flag=true;
  }
}
