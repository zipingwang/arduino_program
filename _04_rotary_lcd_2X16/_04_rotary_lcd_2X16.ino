
////////////////////////////////////////////////
//
// This code may be freely used and copied.
//
// Gareth Davies - June 2012
//
////////////////////////////////////////////////

#include <LiquidCrystal.h>

//first connect backlight pin to potential meter, connect contrast pin to potential meter
LiquidCrystal lcd(12, 11, 5, 4, 7, 6); // Select pins for LCD module

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

volatile int preEncoderValue=0;
volatile int currentIndex=0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;


void setup()
{   
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("hello");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("hello2     ");
  
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);
  displayLcd(0);  
}

void loop()
{   
  // lcd.setCursor(0, 0);
  //lcd.print("hello");
  //delay(1000);
  Serial.println("encoderValue");
  Serial.println(encoderValue);
  Serial.println("PreencoderValue");
  Serial.println(preEncoderValue);
  delay(3000);
}

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
  { 
   encoderValue ++;
  }
  
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) 
  {
    encoderValue --;  
  }

  lastEncoded = encoded; //store this value for next time 
 
  int index=0;
  if((preEncoderValue>=encoderValue+4) || (preEncoderValue==0 && encoderValue==-2) )
  {    
     currentIndex++;
      if(currentIndex>4)
        currentIndex=4;
   
     displayLcd(currentIndex);
     preEncoderValue=encoderValue;    
  }else if((preEncoderValue<=encoderValue-4) || (preEncoderValue==0 && encoderValue==2) )
  {

      currentIndex--;
      if(currentIndex<0)
        currentIndex=0;
      
      displayLcd(currentIndex);
      preEncoderValue=encoderValue;    
  }  
}

void displayLcd(int index)
{
  switch(index)
  {
    case 0:
       lcd.setCursor(0, 0);
       lcd.print(">  A   ");
       lcd.setCursor(0, 1);
       lcd.print("  B   ");
      break;
    case 1:
       lcd.setCursor(0, 0);
       lcd.print(">  B   ");
       lcd.setCursor(0, 1);
       lcd.print("  C   ");
      break;
    case 2:
       lcd.setCursor(0, 0);
       lcd.print(">  C   ");
       lcd.setCursor(0, 1);
       lcd.print("  D   ");
      break;
    case 3:
       lcd.setCursor(0, 0);
       lcd.print(">  D   ");
       lcd.setCursor(0, 1);
       lcd.print("  E   ");
      break;
    case 4:
       lcd.setCursor(0, 0);
       lcd.print("  D   ");
       lcd.setCursor(0, 1);
       lcd.print(">  E   ");
      break;
    default:
      break;
  }
}
