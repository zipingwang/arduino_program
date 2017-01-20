//datasheet 74HC595
//http://www.arduino.cc/en/Tutorial/ShiftOut
//++++++

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = A0;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 12;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;

//use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
int turnPin=13;
int huataiStopPin=3;

//the smaller the value, the faster, used to delaymicroseconds. 4 steps need larger value than 8 steps
int motorSpeed28 =2000; //1200;  //variable to set stepper speed
int motorSpeedHuataiForward =4000; //1200;  //variable to set stepper speed
int motorSpeedHuataiBackword =4000; //1200;  //variable to set stepper speed

//int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
//int lookup[4] = {B01000, B00100, B00010, B00001};

//////////////////////////////////////////////////////////////////////////////
void setup() {

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  pinMode(turnPin, INPUT_PULLUP);
  pinMode(huataiStopPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop(){   
  
   // 4 steps use a little more current under same real motor speed(motorspeed 4steps *2=motorspeed 8steps). turn motor 0.27ma 4 step. 0.2ma for 8 step. 0.8 ma for huatai 4 steps, 0.7 ma for huatai 8 steps
   
   //positive clockwise
   //set turn motor to start position
    goSteps(500, false, 0, 1, 2, 3, 1, 1, motorSpeed28, turnPin);
    sleepMotor(1);
    delay(1000);
    
    //huatai forward, far from motor
    goSteps(-500, false, 4, 6, 5, 7, 1, 1, motorSpeedHuataiForward,huataiStopPin);
    sleepMotor(1);
    delay(1000);
    
    //huatai backward, close to motor
    //滑台返回的距离和速度（motorSpeedHuatai）以及4步或8步无关 只和第一个参数(steps)有关. maximum steps is 95 for huatai 3D0330A1K
    //first parameter 90=53mm. 45=26.5mm   1.7=1mm
    goSteps(76, false, 4, 6, 5, 7, 1, 1, motorSpeedHuataiBackword,-1);
    sleepMotor(1);
    delay(1000);
    
    //negative anticclockwise
    //set turn motor to pull out salt position
    goSteps(-330, false, 0, 1, 2, 3, 1, 1, motorSpeed28, -1);
    sleepMotor(1);
    delay(1000); 
}

//pin1 is A+, pin2 is B+, pin3 is A-, pin4 is B-, first shift(connected to arduino) register is 1 for shiftsequence. when stopPin is low, then stop, stopPin -1 will not check stoppin
//n4or8 is 4 or 8 pulse for one turn, 4 pulse is faster then 8 pulse if motor speed is same(motorSpeed is used in delayMicroseconds(motorSpeed);)
void goSteps(int steps, bool n4or8, int pin1, int pin2, int pin3, int pin4, int shiftSequence, int totalShift, int motorSpeed, int stopPin) //4or8 true is 4, false is 8
{
  if(steps>0)
  {
    if(n4or8)
    {
      for(int icount=0; icount<steps; icount++)
      {    
        if(shouldStop(stopPin))
          return;
       goOnePulse( 1<<pin1, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
         if(shouldStop(stopPin))
          return;
        goOnePulse( 1<<pin2, shiftSequence, totalShift);  
         delayMicroseconds(motorSpeed);
           if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin3, shiftSequence, totalShift); 
         delayMicroseconds(motorSpeed);
           if(shouldStop(stopPin))
          return;
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
               if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin1, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin1 |  1<<pin2, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin2, shiftSequence, totalShift);  
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin2 |  1<<pin3, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin3, shiftSequence, totalShift); 
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin3 |  1<<pin4, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
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
               if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin4, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
        goOnePulse( 1<<pin3, shiftSequence, totalShift);  
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin2, shiftSequence, totalShift); 
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
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
               if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin4, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin4 |  1<<pin3, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin3, shiftSequence, totalShift);  
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin2 |  1<<pin3, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin2, shiftSequence, totalShift); 
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
         goOnePulse( 1<<pin2 |  1<<pin1, shiftSequence, totalShift);     
         delayMicroseconds(motorSpeed);
                if(shouldStop(stopPin))
          return;
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


void sleepMotor(int totalShift)
{
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    if(totalShift==2)
    {     
       shiftOut(dataPin, clockPin, MSBFIRST, 0);  //second shift
       shiftOut(dataPin, clockPin, MSBFIRST, 0);  //first shift      
    }
    else
    {
       shiftOut(dataPin, clockPin, MSBFIRST, 0);  
    }
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);  
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

bool shouldStop(int stopPin)
{
  if(stopPin==-1)
    return false;
  if(digitalRead(stopPin)==LOW)
    return true;
  else
    return false;
}
