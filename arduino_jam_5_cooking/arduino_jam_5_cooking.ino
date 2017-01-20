//datasheet 74HC595
//http://www.arduino.cc/en/Tutorial/ShiftOut
//++++++
#include <ShiftMotor.h>
#include "LCD12864RSPI.h"
#include <Keypad.h>

//begin motor definition
//Motor(controlled by shift register)
ShiftMotor ShiftMotor(A1, A2, A0,2);  

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = A1;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = A2;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = A0;


//int turnPinSalt=3;
//int stopPinHuataiSalt=4;

int turnPinTotal=5;
int stopPinTotal=6;
int noStopPin=-1;


//Motor speed
//同样绝对速度下，滑台8步比4步响。慢比快响, 慢不容易阻塞.
//盒子的外孔不能过大，空隙不能超过盐颗粒的大小，不然会堵住。9v比5v的力大
//the smaller the value, the faster, used to delaymicroseconds. 4 steps need larger value than 8 steps
int motorSpeed28 =1500; //1200;  //variable to set stepper speed
int motorSpeedHuataiForward =2000; //1200;  //variable to set stepper speed
int motorSpeedHuataiBackword =2000; //1200;  //variable to set stepper speed

//int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
//int lookup[4] = {B01000, B00100, B00010, B00001};
//end motor definition

//inducite cookplaat
int pinCookingPlateOnOff = 42;
int pinCookingPlateModeSwitch=44;
int pinCookingPlateTempraturePlus=46;
int pinCookingPlateTemperatureMin=48;
//end inducite cookplaat

//begin lcd difinition
/*
this LCD  Arduino example use SPI(serial mode)
PIN1 GND = GND
PIN2 VCC = 5V
//if want to use different port for RS, RW and EN. change LCD12864RSPI.h and arduino sketch code.
Pin4 RS(CS) (D/I in frizting)  latchpin = 31; 
Pin5 RW(SID) datapin= 33; 
Pin 6 EN(CLK) clockpin= 35;
PIN15 PSB = GND;  //serial mode
有乱码，有时不能正确显示，set RST(Pin 17) LOW.
Pin 19 +5;
Pin 20 GND;
*/

#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )
unsigned char pleaseChooseMenu[]="1. Cooking egg\n2. spaghetti";
unsigned char cookingEggMsg[]="busy...";
unsigned char cookingSpaghettiMsg[]="busy...";
unsigned char cookingDone[]="Done.";
//end lcd difinition

//begin keypad 4x4
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','4','7', '*'},
  {'2','5','8', '0'},
  {'3','6','9', '#'},
  {'A','B','C', 'D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 22, 24, 26, 28 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 30, 32, 34, 36}; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//end keypad 4x4

//////////////////////////////////////////////////////////////////////////////

void setup() {
  //pinMode(turnPinSalt, INPUT_PULLUP);
  //pinMode(stopPinHuataiSalt, INPUT_PULLUP);
  
  //use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
  pinMode(turnPinTotal, INPUT_PULLUP);
  pinMode(stopPinTotal, INPUT_PULLUP);
  
  
   //initialize lcd
  LCDA.Initialise(); // 屏幕初始化
  delay(100);  
  //LCDA.DisplayString(0,0,cookingMachineStartMenu, AR_SIZE(cookingMachineStartMenu));//第一行第1格开始 
  displayMainMenu();
  Serial.begin(9600); 
    
  //------ region set motor begin state
   //set turn motor salt to start position
   // ShiftMotor.goSteps(300, false, 0, 1, 2, 3, 1, motorSpeed28, turnPinSalt);
   // ShiftMotor.sleepMotor();
   // ShiftMotor.goSteps(-300, false, 0, 1, 2, 3, 1, motorSpeed28, turnPinSalt);
   // ShiftMotor.sleepMotor();
    
    
    //total motor to start position
    ShiftMotor.goSteps(500, false, 0, 1, 2, 3, 2, motorSpeed28, turnPinTotal); //stopPinTotal=6;
    ShiftMotor.sleepMotor();
    delay(1000);
    
     //set final turn motor to start position
    ShiftMotor.goSteps(500, false, 4, 5, 6, 7, 2, motorSpeed28, stopPinTotal); //turnPinTotal=5;
    ShiftMotor.sleepMotor();
    delay(1000);    
    
  //------ end region set motor begin state
  
   //moveSalt();  
   
   takeSalt();
  
 
}

 boolean beginInput=false;
 int inputNumber=0;
 int charLength=0;
 
 void displayMainMenu()
 {
   unsigned char cookingMachineStartMenuLine1[]="Menu";
   unsigned char cookingMachineStartMenuLine2[]="1. Cooking egg";
   unsigned char cookingMachineStartMenuLine3[]="2. spaghetti";
   LCDA.DisplayString(0,0,cookingMachineStartMenuLine1, AR_SIZE(cookingMachineStartMenuLine1));//第一行第1格开始 
   LCDA.DisplayString(1,0,cookingMachineStartMenuLine2, AR_SIZE(cookingMachineStartMenuLine2));//第一行第1格开始 
   LCDA.DisplayString(2,0,cookingMachineStartMenuLine3, AR_SIZE(cookingMachineStartMenuLine3));//第一行第1格开始 
 }
 
 void loop()
 {
     displayMainMenu();
     delay(2000);
 }
 

 void loop2()
{
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case 'A': //cooking start menu
         beginInput=true;
         LCDA.CLEAR();//清屏
         delay(200);
         displayMainMenu();
        break;
      case '1':
        if(beginInput==true)
        {
          cookingEgg();
          beginInput=false;
        }        
        break;    
      case '2':
        if(beginInput==true)
        {
          cookingSpaghetti();
          beginInput=false;
        }      
      default:
        //LCDA.CLEAR();//清屏
        //LCDA.DisplayString(0,2, key);//第一行第三格开始，显示文字极客工坊
        Serial.println(key);
    }
  }
}

void takeSalt(){   
    
   //positive clockwise
   //set turn motor to start position
    ShiftMotor.goSteps(200, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
    ShiftMotor.sleepMotor();
    delay(1000);
     
    //negative anticclockwise
    //set turn motor to pull out salt position
    ShiftMotor.goSteps(-200, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
    ShiftMotor.sleepMotor();
    delay(5000); 
    
    moveSalt();
}


//cooking
void cookingEgg()
{
   LCDA.CLEAR();//清屏
    delay(200);
    LCDA.DisplayString(0,0,cookingEggMsg,AR_SIZE(cookingEggMsg));
    
   //turn on
  pinMode(pinCookingPlateOnOff, OUTPUT); 
  digitalWrite(pinCookingPlateOnOff, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(200); 
  pinMode(pinCookingPlateOnOff, INPUT);
  delay(1000);
  
  //switch temperature mode 
  //default is 1200 degree
  pinMode(pinCookingPlateModeSwitch, OUTPUT); 
  digitalWrite(pinCookingPlateModeSwitch, LOW);
  delay(200);
  pinMode(pinCookingPlateModeSwitch, INPUT);
  delay(1000);
  
    //raise temprature to 1400 degree
   pinMode(pinCookingPlateTempraturePlus, OUTPUT); 
   digitalWrite(pinCookingPlateTempraturePlus, LOW);
   delay(200);
   pinMode(pinCookingPlateTempraturePlus, INPUT);
   
   
   takeSalt();
   
   delay(30000); //cooking time
   
   //lower temperature
   // pinMode(pinCookingPlateTemperatureMin, OUTPUT); 
   // digitalWrite(pinCookingPlateTemperatureMin, LOW);
   // delay(200);
   // pinMode(pinCookingPlateTemperatureMin, INPUT);
     
    //turn off
   pinMode(pinCookingPlateOnOff, OUTPUT); 
   digitalWrite(pinCookingPlateOnOff, LOW); 
   delay(200);   
   pinMode(pinCookingPlateOnOff, INPUT);
   delay(000);
   
    LCDA.CLEAR();//清屏
    delay(200);
    LCDA.DisplayString(0,0,cookingDone,AR_SIZE(cookingDone));
}

void cookingSpaghetti()
{
    LCDA.CLEAR();//清屏
    delay(200);
    LCDA.DisplayString(0,0,cookingSpaghettiMsg,AR_SIZE(cookingSpaghettiMsg));
  
    takeSalt();
    
    delay(3000);
  
    LCDA.CLEAR();//清屏
    delay(200);
    LCDA.DisplayString(0,0,cookingDone,AR_SIZE(cookingDone));
}

void moveSalt()
{
    //move
    ShiftMotor.goSteps(-260, false, 4, 5, 6, 7, 2, motorSpeed28, noStopPin); //move
    ShiftMotor.sleepMotor();
    delay(1000);
    
    //turn
    ShiftMotor.goSteps(-300, false, 0, 1, 2, 3, 2, motorSpeed28, noStopPin); //final turn
    ShiftMotor.sleepMotor();
    delay(1000);
    
     //turn back
    ShiftMotor.goSteps(500, false, 0, 1, 2, 3, 2, motorSpeed28, turnPinTotal); //final turn
    ShiftMotor.sleepMotor();
    delay(1000);
    
    //move back
    ShiftMotor.goSteps(500, false, 4, 5, 6, 7, 2, motorSpeed28, stopPinTotal); //move
    ShiftMotor.sleepMotor();
    delay(1000);
}
