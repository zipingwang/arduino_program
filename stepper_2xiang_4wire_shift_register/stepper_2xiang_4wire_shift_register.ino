// This Arduino example demonstrates bidirectional operation of a 
// 28BYJ-48, using a ULN2003 interface board to drive the stepper.
// The 28BYJ-48 motor is a 4-phase, 8-beat motor, geared down by
// a factor of 68. One bipolar winding is on motor pins 1 & 3 and
// the other on motor pins 2 & 4. The step angle is 5.625/64 and the 
// operating Frequency is 100pps. Current draw is 92mA. 
////////////////////////////////////////////////

//declare variables for the motor pins
int motorPin1 = 0;  // A+ // Blue   - 28BYJ48 pin 1
int motorPin2 =5;// B+ 9;    // Pink   - 28BYJ48 pin 2
int motorPin3 =6; // A- 10;    // Yellow - 28BYJ48 pin 3
int motorPin4 = 7; //B-   // Orange - 28BYJ48 pin 4
                        // Red    - 28BYJ48 pin 5 (VCC)
//700, 800
int motorSpeed =2000; //1200;  //variable to set stepper speed // if it 4 steps enlarge the number. 8 steps change it to smaller value

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 12;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;

//////////////////////////////////////////////////////////////////////////////
void setup() {

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
}

//////////////////////////////////////////////////////////////////////////////
void loop(){ 
    goSteps(256, false, 1, 2, 3, 4, 2, 2);
    delay(1000);
    goSteps(-256, false, 1, 2, 3, 4, 2, 2);
    delay(1000);
    goSteps(512, true, 1, 2, 3, 4, 1, 2);
    delay(1000);
    goSteps(-512, true, 1, 2, 3, 4, 1, 2);
    delay(1000); 
}



void goOnePulse(int value)
{
 // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, value);  
    //shiftOut(dataPin, clockPin, LSBFIRST, value); 

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    //delay(500);
}

void goOnePulse(int value, int shiftSequence, int totalShift)
{
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    if(totalShift==2)
    {
      if(shiftSequence==1)
      {
         shiftOut(dataPin, clockPin, MSBFIRST, 0);  //second shift
         shiftOut(dataPin, clockPin, MSBFIRST, value);  //first shift
      }
      else
      {
         shiftOut(dataPin, clockPin, MSBFIRST, value);  //second shift
         shiftOut(dataPin, clockPin, MSBFIRST, 0);  //first shift
      }
    }
    else
    {
          shiftOut(dataPin, clockPin, MSBFIRST, value);  
    }
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    //delay(500);
}

//pin1 is A+, pin2 is B+, pin3 is A-, pin4 is B-
void goSteps(int steps, bool n4or8, int pin1, int pin2, int pin3, int pin4)
{
  goSteps(steps, n4or8, pin1, pin2, pin3, pin4, 1, 1);
}

//pin1 is A+, pin2 is B+, pin3 is A-, pin4 is B-, first shift(connected to arduino) register is 1 for shiftsequence.
void goSteps(int steps, bool n4or8, int pin1, int pin2, int pin3, int pin4, int shiftSequence, int totalShift) //4or8 true is 4, false is 8
{
  if(steps>0)
  {
    if(n4or8)
    {
      for(int icount=0; icount<steps; icount++)
      {    
       goOnePulse( 1<<pin1, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
        goOnePulse( 1<<pin2, shiftSequence, totalShift);  
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin3, shiftSequence, totalShift); 
         delayMicroseconds(motorSpeed);
        goOnePulse( 1<<pin4, shiftSequence, totalShift);
         delayMicroseconds(motorSpeed);
        //shiftOut(dataPin, clockPin, LSBFIRST, value);   
        //take the latch pin high so the LEDs will light up:
        digitalWrite(latchPin, HIGH);
        // pause before next value:
        //delay(500);
      }  
    }
    else
    {
      for(int icount=0; icount<steps; icount++)
      {    
         goOnePulse( 1<<pin1, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin1 |  1<<pin2, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin2, shiftSequence, totalShift);  
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin2 |  1<<pin3, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin3, shiftSequence, totalShift); 
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin3 |  1<<pin4, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin4, shiftSequence, totalShift);
         delayMicroseconds(motorSpeed);
        //shiftOut(dataPin, clockPin, LSBFIRST, value); 
    
        //take the latch pin high so the LEDs will light up:
        digitalWrite(latchPin, HIGH);
        // pause before next value:
        //delay(500);
      }  
    }
  }
  else //negative
  {
    if(n4or8)
    {
      for(int icount=0; icount>steps; icount--)
      {    
         goOnePulse( 1<<pin4, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
        goOnePulse( 1<<pin3, shiftSequence, totalShift);  
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin2, shiftSequence, totalShift); 
         delayMicroseconds(motorSpeed);
        goOnePulse( 1<<pin1, shiftSequence, totalShift);
         delayMicroseconds(motorSpeed);
        //shiftOut(dataPin, clockPin, LSBFIRST, value);   
        //take the latch pin high so the LEDs will light up:
        digitalWrite(latchPin, HIGH);
        // pause before next value:
        //delay(500);
      }  
    }
    else
    {
      for(int icount=0; icount>steps; icount--)
      {    
         goOnePulse( 1<<pin4, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin4 |  1<<pin3, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin3, shiftSequence, totalShift);  
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin2 |  1<<pin3, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin2, shiftSequence, totalShift); 
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin2 |  1<<pin1, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
         goOnePulse( 1<<pin1, shiftSequence, totalShift);
         delayMicroseconds(motorSpeed);
        //shiftOut(dataPin, clockPin, LSBFIRST, value); 
    
        //take the latch pin high so the LEDs will light up:
        digitalWrite(latchPin, HIGH);
        // pause before next value:
        //delay(500);
      }  
    }
  }
}
