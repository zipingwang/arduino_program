// This Arduino example demonstrates bidirectional operation of a 
// 28BYJ-48, using a ULN2003 interface board to drive the stepper.
// The 28BYJ-48 motor is a 4-phase, 8-beat motor, geared down by
// a factor of 68. One bipolar winding is on motor pins 1 & 3 and
// the other on motor pins 2 & 4. The step angle is 5.625/64 and the 
// operating Frequency is 100pps. Current draw is 92mA. 
////////////////////////////////////////////////

//declare variables for the motor pins
//motor driver pin 1->8, pin 2 ->9, pin 3 ->10, pin 4->11
int motorPin1 = 8;   // A+
int motorPin2 =10;   // B+
int motorPin3 =9;    // A-
int motorPin4 = 11;  // B-
                        

int motorSpeed =5000; //for 4 steps: minimun 1500, how smaller the number,how faster. 16385 is the maximun. then restart, e.g 18385=2000
 //for 8 steps: minimun 600, how smaller the number,how faster. 16385 is the maximun. then restart, e.g 18385=2000. 5000 could be the best
 
int count = 0;          // count of steps made
int countsperrev =50;// how many rotation, how long distance it will move.

int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
int steps=8;

//int steps=4;
//int lookup[4] = {B01000, B00100, B00010, B00001};


//////////////////////////////////////////////////////////////////////////////
void setup() {
  //declare the motor pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  Serial.begin(9600);
}

//////////////////////////////////////////////////////////////////////////////
void loop(){
  if(count < countsperrev )
    clockwise();
  else if (count == countsperrev * 2)
    count = 0;
  else
    anticlockwise();
  count++;
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

