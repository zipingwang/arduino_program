
#include "LCD12864RSPI.h"

//#include <avr/pgmspace.h>
//const char pad[500] PROGMEM = { 0 };

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;
int encoderPushPin = 4;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

volatile int preEncoderValue=0;
volatile int currentIndex=0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )

void setup()
{    
  Serial.begin(9600);
  
  LCDA.Initialise(); // 屏幕初始化
  
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  
  digitalWrite(encoderPushPin, HIGH); //turn pullup resistor on
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
  //Serial.println("encoderValue");
  //Serial.println(encoderValue);
  //Serial.println("PreencoderValue");
  //Serial.println(preEncoderValue);
  //delay(3000);   
  
  if(digitalRead(encoderPushPin)==HIGH)
  {
    //button is not being pushed
    //displayLineInLcd(0, "not pushed");
  }
  else
  {
     //button is being pushed
     char buf[sizeof(String(currentIndex))];
     snprintf(buf, sizeof(buf), "%d", currentIndex);
     displayLineInLcd(0, buf);
  } 
 
 //displayLineInLcd(0, "not pushed");
 //displayLineInLcd(0, "pushed");
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
      if(currentIndex>2)
        currentIndex=2;
   
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
       //lcd 12864 can display max 16 char in one line
       unsigned char cookingMachineStartMenuLine1[16]="";
       unsigned char cookingMachineStartMenuLine2[16]="";
       unsigned char cookingMachineStartMenuLine3[16]="";
       unsigned char cookingMachineStartMenuLine4[16]="";
       int line1Length=0;
       int line2Length=0;
       int line3Length=0;
       int line4Length=0;
  switch(index)
  {
    case 0:
       //strncpy((char *)cookingMachineStartMenuLine1, "Menu", strlen("Menu"));
       //line1Length=strlen("Menu");
       displayLineInLcd(0, "Menu");
       displayLineInLcd(1, ">1 cooking egg");
       displayLineInLcd(2, " 2 spaghetti");    
       displayLineInLcd(3, " 3 wok");    
       break;
    case 1:      
       displayLineInLcd(0, "Menu");
       displayLineInLcd(1, " 1 cooking egg");
       displayLineInLcd(2, ">2 spaghetti");  
       displayLineInLcd(3, " 3 wok");    
      break; 
    case 2:      
       displayLineInLcd(0, "Menu");
       displayLineInLcd(1, " 1 cooking egg");
       displayLineInLcd(2, " 2 spaghetti");   
       displayLineInLcd(3, ">3 wok");    
       break;
    default:
      //displayLineInLcd(0, 
      break;
  }
}

//first line nr is 0
void displayLineInLcd(int lineNr, char * text)
{
    LCDA.DisplayString(lineNr,0, (unsigned char *)"                ", 16);//第一行第1格开始 
    LCDA.DisplayString(lineNr,0, (unsigned char *)text, strlen(text));//第一行第1格开始 
}
