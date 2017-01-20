//datasheet 74HC595
//http://www.arduino.cc/en/Tutorial/ShiftOut
//++++++
#include <ShiftMotor.h>
ShiftMotor ShiftMotor(A0, 12,11,2);  

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = A0;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 12;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;

//use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
int turnPinSalt=13;
int stopPinHuataiSalt=3;

int turnPinTotal=9;
int stopPinTotal=8;

//同样绝对速度下，滑台8步比4步响。慢比快响, 慢不容易阻塞
//the smaller the value, the faster, used to delaymicroseconds. 4 steps need larger value than 8 steps
//ACT motor. 8步比4步的电流要大，4步比8步声音听起来要舒服点。 不知哪个好。1.8度的步进电机50个脉冲是一圈360度，如果是4步就是200个脉冲， 8步就是400个脉冲。
//速度900（delay的时间）是4步能设定的最小值（最快），在12v下测的ACT和SUMTOR。小于900马达没时间转够
//用L298N驱动, SUMTOR马达2和3要换过来，ACT和BJ28不用。
//ACT和BJ28 ShiftMotor.goSteps(500, false, 0, 1, 2, 3, 1, motorSpeed28, turnPinSalt);
//SUMTOR马达 ShiftMotor.goSteps(500, false, 0, 2, 1, 3, 1, motorSpeed28, turnPinSalt);
//74HC595 shift register最多电压是5v，超过9v可能会烧掉
//如果在loop里有很多的Serial.print/write, 可能会影响马达工作，肯能serial传速不够快，机子要停留比较长的时间

int motorSpeed28 =2000; //1200;  //variable to set stepper speed
int motorSpeedHuataiForward =900; //4000; //1200;  //variable to set stepper speed
int motorSpeedHuataiBackword =800; //1200;  //variable to set stepper speed
int noStopPin=-1;

//int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
//int lookup[4] = {B01000, B00100, B00010, B00001};

//////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(turnPinSalt, INPUT_PULLUP);
  pinMode(stopPinHuataiSalt, INPUT_PULLUP);
  
  //pinMode(turnPinTotal, INPUT_PULLUP);
  //pinMode(stopPinTotal, INPUT_PULLUP);
  
  /*
  //------ region set begin state
   //set turn motor salt to start position
    ShiftMotor.goSteps(500, false, 0, 1, 2, 3, 1, motorSpeed28, turnPinSalt);
    ShiftMotor.sleepMotor();
    
    //huatai salt forward, far from motor
    ShiftMotor.goSteps(-500, false, 4, 6, 5, 7, 1, motorSpeedHuataiForward, stopPinHuataiSalt);
    ShiftMotor.sleepMotor();
    delay(1000);
    
    //total motor to start position
    ShiftMotor.goSteps(500, false, 0, 1, 2, 3, 2, motorSpeed28, stopPinTotal);
    ShiftMotor.sleepMotor();
    delay(1000);
    
     //set final turn motor to start position
    ShiftMotor.goSteps(500, false, 4, 5, 6, 7, 2, motorSpeed28, turnPinTotal);
    ShiftMotor.sleepMotor();
    delay(1000);
    
    //
    delay(1000);
  //------ end region set begin state
  */
  Serial.begin(9600);
}

void loop(){
  //Serial.write("hello");
  delay(1000);
    // 4 steps use a little more current under same real motor speed(motorspeed 4steps *2=motorspeed 8steps). turn motor 0.27ma 4 step. 0.2ma for 8 step. 0.8 ma for huatai 4 steps, 0.7 ma for huatai 8 steps
   
   //-------- region take salt
   //positive clockwise
   //set turn motor to start position
   // ShiftMotor.goSteps(500, false, 0, 1, 2, 3, 1, motorSpeed28, turnPinSalt);
   // ShiftMotor.sleepMotor();
   // delay(1000);
    
    //huatai forward, far from motor
    //短的长的滑台方向相反？长的滑板负的
    //  ShiftMotor.goSteps(200, true, 4, 6, 5, 7, 1, motorSpeedHuataiForward, stopPinHuataiSalt);
    //ShiftMotor.goSteps(50, false, 4, 6, 5, 7, 1, motorSpeedHuataiForward*4, stopPinHuataiSalt);
    //ShiftMotor.goSteps(50, false, 4, 6, 5, 7, 1, motorSpeedHuataiForward*2, stopPinHuataiSalt);
    //ShiftMotor.goSteps(600, false, 4, 6, 5, 7, 1, motorSpeedHuataiForward, stopPinHuataiSalt);
    //ShiftMotor.goSteps(50, false, 4, 6, 5, 7, 1, motorSpeedHuataiForward*2, stopPinHuataiSalt);
    //ShiftMotor.goSteps(50, false, 4, 6, 5, 7, 1, motorSpeedHuataiForward*4, stopPinHuataiSalt);
    //ShiftMotor.sleepMotor();
    delay(1000);
    
    //huatai backward, close to motor
    //滑台返回的距离和速度（motorSpeedHuatai）以及4步或8步无关 只和第一个参数(steps)有关. maximum steps is 95 for huatai 3D0330A1K
    //first parameter 90=53mm. 45=26.5mm   1.7=1mm
    //ShiftMotor.goSteps(-76, true, 4, 6, 5, 7, 1, motorSpeedHuataiBackword, noStopPin);
    //ShiftMotor.sleepMotor();
    //ShiftMotor.goSteps(10, true, 4, 6, 5, 7, 1, motorSpeedHuataiBackword*2, noStopPin);
    
   // ShiftMotor.goSteps(50, true, 4, 6, 5, 7, 1, motorSpeedHuataiBackword, noStopPin);
    //ShiftMotor.goSteps(10, true, 4, 6, 5, 7, 1, motorSpeedHuataiBackword*2, noStopPin);
    
    //ShiftMotor.sleepMotor();
    
    goSteps2Motor(true, 2, motorSpeedHuataiForward, 500,  4, 6, 5, 7, 1, 
                                                              500,  0, 2, 1, 3, 1 );
    sleepMotor(2);
    delay(1000);
}

void goSteps2Motor(bool n4or8,int totalShift, int motorSpeed, int stepsM1,  int pin1M1, int pin2M1, int pin3M1, int pin4M1, int shiftSequenceM1, 
                                                              int stepsM2,  int pin1M2, int pin2M2, int pin3M2, int pin4M2, int shiftSequenceM2 ) //4or8 true is 4, false is 8
{
  float proportion=1.0;
  if(abs(stepsM1)>=abs(stepsM2))
  {
     proportion=abs(stepsM2);
     proportion=proportion/abs(stepsM1);
     doGoSteps2Motor(n4or8, totalShift, motorSpeed, proportion, stepsM1,  pin1M1, pin2M1, pin3M1, pin4M1, shiftSequenceM1, 
                                                                stepsM2,  pin1M2, pin2M2, pin3M2, pin4M2, shiftSequenceM2);
  }
  else
  {
      proportion=abs(stepsM1);
      proportion=proportion/abs(stepsM2);
      doGoSteps2Motor(n4or8, totalShift, motorSpeed, proportion, stepsM2,  pin1M2, pin2M2, pin3M2, pin4M2, shiftSequenceM2,
                                                                 stepsM1,  pin1M1, pin2M1, pin3M1, pin4M1, shiftSequenceM1);
  }
}

int tryMotor2OnePluse(bool direction, bool n4or8,int totalShift,  int shiftSequenceM2, int stepCountM2, int stepCountM1, float proportion,
                               int pin1M2, int pin2M2, int pin3M2, int pin4M2)
{
  int pin=-1;
  float tempValue;
  tempValue=stepCountM1;
  tempValue=tempValue * proportion;
   //Serial.print("t");
  // Serial.print(tempValue);
   //Serial.print("sm2");
   //Serial.print(stepCountM2);
  // delay(2000);
  if(tempValue-stepCountM2>=1)
  {     // Serial.print("enter tempValue-stepCountM2>=1");
         
        if(direction>0)
        {
          if(n4or8)
          {
           // Serial.print("enter n4or8");
             int whichStep= stepCountM2 % 4;
            // Serial.print("whichStep ");
            // Serial.print(whichStep);
             switch(whichStep)
             {
               case 0:
                 pin=pin1M2;
                 //Serial.print("case0 ");
                 // Serial.print(pin1M2);
                 //goOnePulse( 1<<pin1M2, shiftSequenceM2, totalShift);  
               break;
               case 1:
                 //goOnePulse( 1<<pin2M2, shiftSequenceM2, totalShift);  
                 pin=pin2M2;
               break;
               case 2:
                 //goOnePulse( 1<<pin3M2, shiftSequenceM2, totalShift);  
                   pin=pin3M2;
               break;
               case 3:
                 //goOnePulse( 1<<pin4M2, shiftSequenceM2, totalShift);  
                   pin=pin4M2;
               break;
               default:
               break;
             }
          }
        }
        //currentStepCountM2++;
  }
  //Serial.print("pin ");
  // Serial.print(pin);
  return pin;
}

void doGoSteps2Motor(bool n4or8,int totalShift, int motorSpeed, float proportion, 
                    int stepsM1,  int pin1M1, int pin2M1, int pin3M1, int pin4M1, int shiftSequenceM1, 
                    int stepsM2,  int pin1M2, int pin2M2, int pin3M2, int pin4M2, int shiftSequenceM2 ) //4or8 true is 4, false is 8
{  
  int stepCountM2=0;
  int stepCountM1=1;
  int pinM2=-1;
  if(stepsM1>0)
  {
    if(n4or8)
    {
      for(int icount=0; icount<stepsM1; icount++)
      {    
        //if(shouldStop(stopPin))
        //  return;
       pinM2=tryMotor2OnePluse(stepsM2, n4or8, totalShift, shiftSequenceM2, stepCountM2, stepCountM1, proportion,
                               pin1M2, pin2M2, pin3M2, pin4M2);
       if(pinM2>-1)
       {
         stepCountM2++;
         goOnePulse( 1<<pin1M1 | 1<<pinM2 , shiftSequenceM1, totalShift);    
       }
       else
       {
         goOnePulse( 1<<pin1M1, shiftSequenceM1, totalShift);    
       }
       stepCountM1++; 
         delayMicroseconds(motorSpeed);
        // if(shouldStop(stopPin))
        //  return;
         pinM2=tryMotor2OnePluse(stepsM2, n4or8, totalShift, shiftSequenceM2, stepCountM2, stepCountM1, proportion,
                               pin1M2, pin2M2, pin3M2, pin4M2);
        if(pinM2>-1)
       {
         stepCountM2++;
         goOnePulse( 1<<pin2M1 | 1<<pinM2 , shiftSequenceM1, totalShift);    
       }
       else
       {
          goOnePulse( 1<<pin2M1, shiftSequenceM1, totalShift);      
       }
       
        stepCountM1++; 
         delayMicroseconds(motorSpeed);
         //  if(shouldStop(stopPin))
         // return;
          pinM2=tryMotor2OnePluse(stepsM2, n4or8, totalShift, shiftSequenceM2, stepCountM2, stepCountM1, proportion,
                               pin1M2, pin2M2, pin3M2, pin4M2);
        if(pinM2>-1)
       {
         stepCountM2++;
         goOnePulse( 1<<pin3M1 | 1<<pinM2 , shiftSequenceM1, totalShift);    
       }
       else
       {
           goOnePulse( 1<<pin3M1, shiftSequenceM1, totalShift);  
       }
        
         stepCountM1++; 
         delayMicroseconds(motorSpeed);
         //  if(shouldStop(stopPin))
         // return;
        pinM2=tryMotor2OnePluse(stepsM2, n4or8, totalShift, shiftSequenceM2, stepCountM2, stepCountM1, proportion,
                               pin1M2, pin2M2, pin3M2, pin4M2);
       if(pinM2>-1)
       {
         stepCountM2++;
         goOnePulse( 1<<pin4M1 | 1<<pinM2 , shiftSequenceM1, totalShift);    
       }
       else
       {
          goOnePulse( 1<<pin4M1, shiftSequenceM1, totalShift); 
       }
        
        stepCountM1++; 
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
      for(int icount=0; icount<stepsM1; icount++)
      {    
               //if(shouldStop(stopPin))
          //return;
         goOnePulse( 1<<pin1M1, shiftSequenceM1, totalShift);     
         delayMicroseconds(motorSpeed);
           //     if(shouldStop(stopPin))
          //return;
         goOnePulse( 1<<pin1M1 |  1<<pin2M1, shiftSequenceM1, totalShift);     
         delayMicroseconds(motorSpeed);
          //      if(shouldStop(stopPin))
          //return;
         goOnePulse( 1<<pin2M1, shiftSequenceM1, totalShift);  
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin2M1 |  1<<pin3M1, shiftSequenceM1, totalShift);     
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin3M1, shiftSequenceM1, totalShift); 
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
          //return;
         goOnePulse( 1<<pin3M1 |  1<<pin4M1, shiftSequenceM1, totalShift);     
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin4M1, shiftSequenceM1, totalShift);
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
      for(int icount=0; icount>stepsM1; icount--)
      {    
        //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin4M1, shiftSequenceM1, totalShift);     
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
        goOnePulse( 1<<pin3M1, shiftSequenceM1, totalShift);  
         delayMicroseconds(motorSpeed);
          //      if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin2M1, shiftSequenceM1, totalShift); 
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
        goOnePulse( 1<<pin1M1, shiftSequenceM1, totalShift);
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
      for(int icount=0; icount>stepsM1; icount--)
      {    
         //      if(shouldStop(stopPin))
        //  return;
         goOnePulse( 1<<pin4M1, shiftSequenceM1, totalShift);     
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin4M1 |  1<<pin3M1, shiftSequenceM1, totalShift);     
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin3M1, shiftSequenceM1, totalShift);  
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin2M1 |  1<<pin3M1, shiftSequenceM1, totalShift);     
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin2M1, shiftSequenceM1, totalShift); 
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin2M1 |  1<<pin1M1, shiftSequenceM1, totalShift);     
         delayMicroseconds(motorSpeed);
         //       if(shouldStop(stopPin))
         // return;
         goOnePulse( 1<<pin1M1, shiftSequenceM1, totalShift);
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
