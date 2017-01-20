#include <multiMotorDriver.h>
#include <ShiftMotor.h>
#include "LCD12864RSPI.h"
#include <Keypad.h>
#include "menu.h"
#include "main.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "multiStepperMotorDriver.h"

//+++++++++++  begin motor definition
//Motor(controlled by shift register)
ShiftMotor shiftMotor= ShiftMotor(A1, A2, A0,2);  
ShiftMotor shiftMotor2= ShiftMotor(A4, A5, A3, 2);

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = A1;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = A2;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = A0;

int noStopPin= NO_STOP_PIN;//-1;

int turnPinTotal=5;
int stopPinTotal=6;
int stopPinMoveOil=7;
int stopPinMoveTemperatorSensor=8;

int stopPinOpenCover=9;
int openCoverMotorPin1=10;
int openCoverMotorPin2=11;

//Motor speed
//同样绝对速度下，滑台8步比4步响。慢比快响, 慢不容易阻塞.
//盒子的外孔不能过大，空隙不能超过盐颗粒的大小，不然会堵住。9v比5v的力大
//the smaller the value, the faster, used to delaymicroseconds. 4 steps need larger value than 8 steps
//if 8 steps, 512 step is one turn. but if charg need more to make one turn. e.g. 600
//2000 speed, 8 ssteps, 6 physical turn is 58seconds. 1200 speed, 8 steps, 6 physical turns is 36 seconds, 1000 speed, 8 steps, 6 physical turns is 32 seconds,
int motorSpeed28 =2000; //1200;  //variable to set stepper speed //800 is smallest value can set for 8 steps, 2000 is good for 8 steps, two small for 4 steps
int motorSpeed42Forward =800; //1200;  //variable to set stepper speed
int motorSpeed42Backword =2000; //1200;  //variable to set stepper speed

//int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
//int lookup[4] = {B01000, B00100, B00010, B00001};
MultiStepperMotorDriver motorGroup1= MultiStepperMotorDriver(A1, A2, A0, 2, 8, motorSpeed28); //(int latchPin, int clockPin, int dataPin, int totalShift, int nSteps, int motorSpeed)
MultiStepperMotorDriver motorGroup2= MultiStepperMotorDriver(A4, A5, A3, 2, 8, motorSpeed28);

Motor motorSalt = Motor(1, 1, false, NO_STOP_PIN, STEPS_PER_TURN);  //Motor(int registerNr, int firstOrSecondBlock, bool isPin2and3Reversed, int stopPin) //first registerNr is 1, first block(0,1,2,3) is 1, second block(4,5,6,7) is 2
Motor motorWeijing = Motor(1, 2, false, NO_STOP_PIN, STEPS_PER_TURN);  
Motor motorSugar = Motor(2, 1, false, NO_STOP_PIN, STEPS_PER_TURN);    

Motor motorTurnTotal = Motor(1, 2, false, turnPinTotal, STEPS_PER_TURN); 
Motor motorMoveTotal = Motor(2, 2, false, stopPinTotal, STEPS_PER_TURN); 
Motor motorMoveOil = Motor(1, 1, false, stopPinMoveOil, STEPS_PER_TURN); 
Motor motorMoveTemperatorSensor = Motor(2, 1, false, stopPinMoveTemperatorSensor, STEPS_PER_TURN); //not used 

Motor ingredientMotorsGroup[3];
//------ end motor

//------------ end motor definition

//++++++++++++ begin inducite cookplaat
//use relay to control de cookplate
int pinCookingPlateOnOff = 22;
int pinCookingPlateMode=24; //heat bottom, when it is nengtou caocaiji
int pinCookingPlatePowerPlus=26; //heat top, when it is nengtou caocaiji
int pinCookingPlatePowerMin=28; //motor, when it is nengtou caocaiji

int pinNengtouHeatBottom=24;
int pinNengtouMotor=26;
int pinNengtouHeatTop=28;
int pinNengtouPreheating=14;
//------------ end inducite cookplaat

//++++++++++++ begin lcd difinition
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
unsigned char cookingEggMsg[]="busy...";
unsigned char cookingSpaghettiMsg[]="busy...";
unsigned char cookingDone[]="Done.";
//--------------- end lcd difinition

/*
//+++++++++++++++ begin keypad 4x4
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
//---------------- end keypad 4x4
*/

//++++++++++++++++ begin rotary encoder
//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;
int encoderPushPin = 4;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

volatile int preEncoderValue=0;
volatile int currentIndex=0;
volatile int currentPage=0;

long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;
//-------------- end rotary encoder

//++++ begin liquid
int pinOil=30;
int pinWater =32;
int pinLaoChou=34;
int pinQianFeng =36;
int pinShenChou =38; //no used yet, no relay pin
int pinJiu =40; //no used yet, no relay pin
int pinMaYou =42; //no used yet, no relay pin
//---- end liquid

//++++ begin mlx90614
 Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 //----- end mlx
 
//++++ begin global variable
 volatile bool busy=false;

 boolean beginInput=false;
 int inputNumber=0;
 int charLength=0; 
 
 long debounceDelay = 1000;
 long lastDebounceTime = 0; 
//---- end global variable

//+++++ begin pin definition
int pinBusyLed=53; //if use relay it is 30
int pinIdleLed=51; //if use relay it is 32
//----- end pin definition

//////////////////////////////////////////////////////////////////////////////

void setup() {   
  Serial.begin(9600);
  Serial2.begin(9600); //for tts communication
  //use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
  pinMode(turnPinTotal, INPUT_PULLUP);
  pinMode(stopPinTotal, INPUT_PULLUP);  
  pinMode(stopPinMoveOil, INPUT_PULLUP);  
  pinMode(stopPinMoveTemperatorSensor, INPUT_PULLUP);  
  pinMode(stopPinOpenCover, INPUT_PULLUP);   

  //cover motor
  pinMode(openCoverMotorPin1, OUTPUT);
  pinMode(openCoverMotorPin2, OUTPUT);
  digitalWrite(openCoverMotorPin1, LOW);
  digitalWrite(openCoverMotorPin2, LOW); 
  
  //rotary Encoder
  pinMode(encoderPushPin, INPUT_PULLUP);
  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
  
  //cooking plate
  pinMode(pinCookingPlateOnOff, OUTPUT);  
  pinMode(pinCookingPlateMode, OUTPUT);  
  pinMode(pinCookingPlatePowerPlus, OUTPUT);  
  pinMode(pinCookingPlatePowerMin, OUTPUT);  
  
  digitalWrite(pinCookingPlateOnOff, HIGH);
  digitalWrite(pinCookingPlateMode, HIGH);
  digitalWrite(pinCookingPlatePowerPlus, HIGH);
  digitalWrite(pinCookingPlatePowerMin, HIGH);

  //nengdou
  pinMode(pinNengtouHeatBottom, OUTPUT);  
  pinMode(pinNengtouHeatTop, OUTPUT);  
  pinMode(pinNengtouMotor, OUTPUT);  
  pinMode(pinNengtouPreheating, INPUT_PULLUP);  
  
  digitalWrite(pinNengtouHeatBottom, HIGH);
  digitalWrite(pinNengtouHeatTop, HIGH);
  digitalWrite(pinNengtouMotor, HIGH); 
  
  //liquid pomp //through relay
  pinMode(pinOil, OUTPUT); 
  digitalWrite(pinOil, HIGH);
  pinMode(pinLaoChou, OUTPUT); 
  digitalWrite(pinLaoChou, HIGH);
  pinMode(pinShenChou, OUTPUT); 
  digitalWrite(pinShenChou, HIGH);
  pinMode(pinWater, OUTPUT); 
  digitalWrite(pinWater, HIGH);
  pinMode(pinJiu, OUTPUT); 
  digitalWrite(pinJiu, HIGH);
  pinMode(pinMaYou, OUTPUT); 
  digitalWrite(pinMaYou, HIGH);
  pinMode(pinQianFeng, OUTPUT); 
  digitalWrite(pinQianFeng, HIGH); 
  
  digitalWrite(stopPinMoveOil, INPUT_PULLUP);
  
  //+++++ begin pin definition
  pinMode(pinBusyLed, OUTPUT); 
  //digitalWrite(pinBusyLed, HIGH); //if it goes through relay, relay low has effect
  digitalWrite(pinBusyLed, LOW);
  pinMode(pinIdleLed, OUTPUT); 
  //digitalWrite(pinIdleLed, HIGH); //if it goes through relay, relay low has effect
  digitalWrite(pinIdleLed, LOW);
 //----- end pin definition
  
   //initialize lcd
  LCDA.Initialise(); // 屏幕初始化
  delay(100);  

  currentPage=PAGE_MAIN;
  currentIndex=0;
  displayMainMenu();
    
  //------ region set motor begin state  
  /* 
    ingredientMotorsGroup[0]=motorSalt; Motor is struct, if assigned motorSalt to the array, a copy of motorSalt is made, and copy to the array. it is not the original one.
    ingredientMotorsGroup[1]=motorSugar;
    ingredientMotorsGroup[2]=motorWeijing;
  */      
    motorGroup1.sleepMotor();
    motorGroup2.sleepMotor();
    
    doSetBusySignOn(); 
         
    //set motors to their start position
    setMotorToStartPosition();
        
    doSetBusySignOff();
    doSetIdleSignOn(); 
    
  //------ end region set motor begin state  
     
   //call updateEncoder() when any high/low changed seen
   //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
   attachInterrupt(0, updateEncoder, CHANGE); 
   attachInterrupt(1, updateEncoder, CHANGE); 
   
   mlx.begin();  
   
   //interrupt 5(pin 18 in arduino mega)
   pinMode(18, INPUT_PULLUP);
   attachInterrupt(5, userHandleRequest, CHANGE);
}

 void displayMainMenu()
 {
   /*
   unsigned char cookingMachineStartMenuLine1[]="Menu";
   unsigned char cookingMachineStartMenuLine2[]="1. Cooking egg";
   unsigned char cookingMachineStartMenuLine3[]="2. spaghetti";
   LCDA.DisplayString(0,0,cookingMachineStartMenuLine1, AR_SIZE(cookingMachineStartMenuLine1));//第一行第1格开始 
   LCDA.DisplayString(1,0,cookingMachineStartMenuLine2, AR_SIZE(cookingMachineStartMenuLine2));//第一行第1格开始 
   LCDA.DisplayString(2,0,cookingMachineStartMenuLine3, AR_SIZE(cookingMachineStartMenuLine3));//第一行第1格开始 
  */
  displayLcd(0, 0);
 }
 
void loop2() {
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");

  Serial.println();
  delay(500);
}
 
 void loop()
 {
   if(digitalRead(encoderPushPin)==HIGH)
   {
     //button is not being pushed
     //displayLineInLcd(0, "not pushed");
   }
   else
   {
     //button is being pushed
     if ((millis() - lastDebounceTime) < debounceDelay)
     {
        return;
     }
     lastDebounceTime = millis();
     
     //char buf[sizeof(String(currentIndex))];
     //snprintf(buf, sizeof(buf), "%d", currentIndex);
     //displayLineInLcd(0, buf);     
  
     if(currentPage==PAGE_MAIN)
     {      
       switch(currentIndex)
       {
         case 0:
           currentPage=PAGE_DISH;
           currentIndex=0;
           displayLcd(currentPage, currentIndex);
           break;      
         case 1:
           currentPage=PAGE_CONFIG;
           currentIndex=0;
           displayLcd(currentPage, currentIndex);         
           break;    
         case 2:
           currentPage=PAGE_POWER_TIME;
           currentIndex=2;
           displayLcd(currentPage, currentIndex);         
           break;    
       }   
     }
     else if(currentPage==PAGE_DISH)
     {     
       switch(currentIndex)
       {
         case 0:
           currentPage=PAGE_MAIN;
           currentIndex=0;
           displayLcd(currentPage, currentIndex);
           break;  
         //case 1:
         //  //cookingEgg();
         //  carryOutCommands(commands[1]);
         //  break;
         default:
           //cookingEgg();
             _adjustTime=0;
            _adjustTimeDone=false;
            
           carryOutCommands(commands[currentIndex]);
           break;
       }      
     }
     else if(currentPage==PAGE_CONFIG)
     {
       switch(currentIndex)
       {
         case 0: //back "back", 
           currentPage=PAGE_MAIN;
           currentIndex=0;
           displayLcd(currentPage, currentIndex);
           break;  
        case 1: //"pomp oil", 
           displayBusyLcd();
           connectRelayPin(pinOil, 5000);
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 2: //"move total",
           displayBusyLcd();
           moveTotal();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 3: //turn salt
           displayBusyLcd();
           doTakeSalt(STEPS_PER_TURN);
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 4: //move oil
           displayBusyLcd();
           moveOilOut();
           delay(2000);
           moveOilIn();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 5: //measure temprature
           //moveTemperatorSensorOut(); //nengtou don't need move arm           
           LCDA.CLEAR();
           displayLineInLcd(0, ">back");    
           
           char output[16];  
   
           _interruptedByUser=false;
           while(!_interruptedByUser) //continue to measure temperture until user press the interrupt button
           {
             dtostrf(mlx.readAmbientTempC(),2,2,output);          
             Serial.print("Ambient = "); Serial.print(output);
             displayLineInLcd(1,  output);      
            
             dtostrf(mlx.readObjectTempC(),2,2,output);  
             displayLineInLcd(2,  output); 
             delay(500); 
           }
           
          // moveTemperatorSensorIn(); //nengtou don't need move arm
            
           currentPage=PAGE_CONFIG_DONE;   
           displayDoneLcd();
                   
           break;        
         case 6:   //"plate on/off",
           displayBusyLcd();
           doTurnOnCookingPlate(); //on off is toggle button
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
         case 7: //"plate mode",
           displayBusyLcd();
           doSwitchCookingPlateToMode();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
         case 8: //plate T + ",
           displayBusyLcd();
           doSetPlatePowerPlus(1);
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
         case 9: //plate T - ",
           displayBusyLcd();
           doSetPlatePowerMin(1);
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
         case 10: // "turn salt(empty)",
           displayBusyLcd();
           
           _interruptedByUser=false;
           while(!_interruptedByUser) //continue to turn until user press the interrupt button
           {
             doTakeSalt(50);
           };
           
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
         case 11: // "turn sugar(empty)",
           displayBusyLcd();
           
           _interruptedByUser=false;
           while(!_interruptedByUser) //continue to turn until user press the interrupt button
           {
             doTakeSugar(50);
           };
           
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
           
         case 12: // "turn weijing(empty)",
           displayBusyLcd();
           
           _interruptedByUser=false;
           while(!_interruptedByUser) //continue to turn until user press the interrupt button
           {
             doTakeWeiJing(50);
           };
           
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
           
         case 13: //turn sugar
           displayBusyLcd();
           doTakeSugar(STEPS_PER_TURN);
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
           
        case 14: //turn weijing
           displayBusyLcd();
           doTakeWeiJing(STEPS_PER_TURN);
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 15: //cover open
           displayBusyLcd();
           doOpenCover();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 16: //cover close
           displayBusyLcd();
           doCloseCover();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 17://    "heat bottom on",
           displayBusyLcd();
           doHeatBottomOn();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
          break;  
        case 18://    "heat top on",
           displayBusyLcd();
           doHeatTopOn();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
            break;
        case 19://     "stir motor 10s",
           displayBusyLcd();
           doStirMotor10s();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
            break;
        case 20://    "heat bottom off",
           displayBusyLcd();
           doHeatBottomOff();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 21://    "heat top off"
           displayBusyLcd();
           doHeatTopOff();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;           
        case 22://     "Preheating"
           displayBusyLcd();
           doPreheating();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break; 
        case 23://     "openCloseRepea"
           displayBusyLcd();
           doOpenCloseRepeat();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;   
        case 24: // "selfTest"   
            displayBusyLcd();        
            doSelfTest();       
            currentPage=PAGE_CONFIG_DONE;
            displayDoneLcd();   
            break;
       }     
     }
     else if(currentPage==PAGE_POWER_TIME)
     {
       //lastDebounceTime = millis();
       switch(currentIndex)
       {
         default:
           int powerTime=atoi(page_power_time[currentIndex]);
           
           doHeatBottomOn();
           doStirMotorOn();
           delay(powerTime *1000);
           doHeatBottomOff();
           doStirMotorOff();
           
           currentPage=PAGE_MAIN;
           currentIndex=0;
           displayLcd(currentPage, currentIndex);        
           break;       
       }
     }       
     else if(currentPage==PAGE_COOKING_DONE)
     {
       //lastDebounceTime = millis();
       switch(currentIndex)
       {
         case 0:
         case 1:
         case 2:
         case 3:
           currentPage=PAGE_DISH;
           currentIndex=0;
           displayLcd(currentPage, currentIndex);         
           break;       
       }
     }       
     else if(currentPage==PAGE_CONFIG_DONE)
     {
       //lastDebounceTime = millis();
       switch(currentIndex)
       {
         case 0:
         case 1:
         case 2:
         case 3:
           currentPage=PAGE_CONFIG;
           currentIndex=0;
           displayLcd(currentPage, currentIndex);         
           break;       
       }
     }     
     else if(currentPage==PAGE_ADJUST_TIME)
     {
       //lastDebounceTime = millis();
       switch(currentIndex)
       {
          default:
          _adjustTime=atoi(page_adjust_time[currentIndex]);
          _adjustTimeDone=true;
          carryOutCommands(_commands);
          break;
       }
     }          
   } 
 }
 
/*
void loop_use_keypad()
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
          //cookingSpaghetti();
          beginInput=false;
        }      
      default:
        //LCDA.CLEAR();//清屏
        //LCDA.DisplayString(0,2, key);//第一行第三格开始，显示文字极客工坊
        Serial.println(key);
    }
  }
}
*/

void moveOilOut(){
   //positive clockwise   
   //512 is one turn(measured by practice)  
   
   moveOilIn(); //always from its start position     
   motorMoveOil.stopPin=NO_STOP_PIN;
   motorGroup2.driveMotors(motorMoveOil, 120);     
}

void moveOilIn(){  
   //positive clockwise   
   //512 is one turn(measured by practice)     
   motorMoveOil.stopPin=stopPinMoveOil;
   motorGroup2.driveMotors(motorMoveOil, -500);   
}

void moveTemperatorSensorOut(){ 
   //positive clockwise   
   //512 is one turn(measured by practice)
    moveTemperatorSensorIn(); //alwarys from start position
    motorMoveTemperatorSensor.stopPin=NO_STOP_PIN;
    motorGroup2.driveMotors(motorMoveTemperatorSensor, 45);
}

void moveTemperatorSensorIn(){ 
   //positive clockwise   
   //512 is one turn(measured by practice)     
   motorMoveTemperatorSensor.stopPin=stopPinMoveTemperatorSensor;
   motorGroup2.driveMotors(motorMoveTemperatorSensor, -500);   
}

void updateEncoder(){
    if(busy==true)
      return;
    
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
       if(currentIndex>PAGE_MAIN_SIZE-1 && currentPage==PAGE_MAIN)
          currentIndex=PAGE_MAIN_SIZE-1;
       else if(currentIndex>PAGE_DISH_SIZE-1 && currentPage==PAGE_DISH)
          currentIndex=PAGE_DISH_SIZE-1;
       else if(currentIndex>PAGE_CONFIG_SIZE-1 && currentPage==PAGE_CONFIG)
          currentIndex=PAGE_CONFIG_SIZE-1;
       else if(currentIndex>PAGE_ADJUST_TIME-1 && currentPage==PAGE_ADJUST_TIME)
          currentIndex=PAGE_ADJUST_TIME-1;
       else if(currentIndex>PAGE_POWER_TIME-1 && currentPage==PAGE_POWER_TIME)
          currentIndex=PAGE_POWER_TIME-1;
       else if(currentIndex>3 && currentPage==PAGE_COOKING_DONE)
          currentIndex=3;
       else if(currentIndex>3 && currentPage==PAGE_CONFIG_DONE)
          currentIndex=3;
          
       displayLcd(currentPage, currentIndex);
       preEncoderValue=encoderValue;    
    }else if((preEncoderValue<=encoderValue-4) || (preEncoderValue==0 && encoderValue==2) )
    {

      currentIndex--;
      if(currentIndex<0)
        currentIndex=0;
      
      displayLcd(currentPage, currentIndex);
      preEncoderValue=encoderValue;    
    } 
  }  

void displayLcd(int currentPage, int index)
{ 
    //lcd 12864 can display max 16 char in one line
        
    if(currentPage==PAGE_MAIN) //main menu
    {
     displayPage(index, page_main, PAGE_MAIN_SIZE);
    }
    else if(currentPage==PAGE_DISH) //dish menu
    {      
     displayPage(index, page_dish, PAGE_DISH_SIZE);
    }
    else if(currentPage==PAGE_COOKING_DONE) //done menu
    {     
      switch(currentIndex)
      {
        case 0:
        case 1:
        case 2:
        case 3:
          displayDoneLcd();
          break;
      }
    }
    else if(currentPage==PAGE_CONFIG) //maintance menu
    {     
      displayPage(index, page_config, PAGE_CONFIG_SIZE);
    }
    else if(currentPage==PAGE_ADJUST_TIME) //adjust time menu
    {     
      displayPage(index, page_adjust_time, PAGE_ADJUST_TIME_SIZE);
    }
    else if(currentPage==PAGE_POWER_TIME) //power time menu
    {     
      displayPage(index, page_power_time, PAGE_POWER_TIME_SIZE);
    }
}

void displayPage(int index, const char* page[], int pageSize)
{
     Serial.println(index);
     LCDA.CLEAR(); 
     for(int i=0; i<4; i++)
     {
       Serial.println((index/4)*4+i);
       if((index/4)*4+i<pageSize)
       {         
           displayLineInLcd(i, page[(index/4)*4+i]);
       }      
     }   
     displayLineSelectedInLcd(index-(index/4)*4); 
}


//first line nr is 0
void displayLineInLcd(int lineNr, char * text) //displayLineInLcd(1, "geek-workshop");
{
    LCDA.DisplayString(lineNr,0, (unsigned char *)"                ", 16);//第一行第1格开始 
    LCDA.DisplayString(lineNr,1, (unsigned char *)text, strlen(text));//第一行第1格开始 
}

void displayLineInLcd(int lineNr, unsigned char * text) //displayLineInLcd(1, show0); //show0 is usinged char array
{
    LCDA.DisplayString(lineNr,0, (unsigned char *)"                ", 16);//第一行第1格开始 
    LCDA.DisplayString(lineNr,1, (unsigned char *)text, strlen((const char*)text));//第一行第1格开始 
}

void displayLineInLcd(int lineNr, const char * text) //displayLineInLcd(1, show0);
{
    LCDA.DisplayString(lineNr,0, (unsigned char *)"                ", 16);//第一行第1格开始 
    LCDA.DisplayString(lineNr,1, (unsigned char *)text, strlen(text));//第一行第1格开始   
}

void displayLineSelectedInLcd(int lineNr) //first line is 0
{   
    LCDA.DisplayString(lineNr,0, (unsigned char *)">", 1);//第一行第1格开始    
}

void displayBusyLcd()
{
  LCDA.CLEAR();
  displayLineInLcd(0, "busy...");
}

void displayDoneLcd()
{
   LCDA.CLEAR();
   displayLineInLcd(0, "done");
   displayLineInLcd(1, ">back");      
   currentIndex=1;   
}

void userHandleRequest()
{
  _requestHandled=true;
  _interruptedByUser=true;
  
  /*
  if(_requestAdjustTime==true)
  {
    _adjustTime=atoi(page_adjust_time[currentIndex]);
    _adjustTimeDone=true;
  }
  */
  
  //open cover and requestHandled use same interrupt, because assume that open cover and requestHandle not be used at same time.
  //_openCoverReached=true;
}

void setMotorToStartPosition()
{  
    motorTurnTotal.stopPin=turnPinTotal;
    motorMoveTotal.stopPin=stopPinTotal;
    motorMoveOil.stopPin=stopPinMoveOil;
    //motorMoveTemperatorSensor.stopPin=stopPinMoveTemperatorSensor;
    
    motorGroup2.driveMotors(motorTurnTotal, STEPS_PER_TURN);                     
    motorGroup2.driveMotors(motorMoveTotal, -STEPS_PER_TURN);
    motorGroup2.driveMotors(motorMoveOil, -STEPS_PER_TURN);
    //motorGroup2.driveMotors(motorMoveTemperatorSensor, -512);
}

void testStepMotors()
{
        MultiStepperMotorDriver motorDriver= MultiStepperMotorDriver(A4, A5, A3, 2, false, motorSpeed28);        
        Motor motors[3];
	Motor motorSalt=Motor(2, false, false, NO_STOP_PIN, -500);

	Motor motorSugar;
	motorSugar.registerNr=1;
	motorSugar.steps=20;
	motorSugar.id=2;
	motorSugar.isPin2and3Reversed=false;
	motorSugar.firstOrSecondBlock=false;

	Motor motorWeijing;
	motorWeijing.registerNr=1;
	motorWeijing.steps=30;
	motorWeijing.id=3;
	motorWeijing.isPin2and3Reversed=true;
	motorWeijing.firstOrSecondBlock=true;

	motors[0]=motorSalt;
	motors[1]=motorSugar;
	motors[2]=motorWeijing;
    
	motorDriver.driveMotors(motors, 3);
}
