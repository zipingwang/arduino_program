#include <multiMotorDriver.h>

//datasheet 74HC595
//http://www.arduino.cc/en/Tutorial/ShiftOut
//++++++

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

int turnPinTotal=5;
int stopPinTotal=6;
int noStopPin= NO_STOP_PIN;//-1;

//Motor speed
//同样绝对速度下，滑台8步比4步响。慢比快响, 慢不容易阻塞.
//盒子的外孔不能过大，空隙不能超过盐颗粒的大小，不然会堵住。9v比5v的力大
//the smaller the value, the faster, used to delaymicroseconds. 4 steps need larger value than 8 steps
//if 8 steps, 512 step is one turn. but if charg need more to make one turn. e.g. 600
int motorSpeed28 =2000; //1200;  //variable to set stepper speed //800 is smallest value can set for 8 steps, 2000 is good for 8 steps, two small for 4 steps
int motorSpeed42Forward =800; //1200;  //variable to set stepper speed
int motorSpeed42Backword =2000; //1200;  //variable to set stepper speed

//int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
//int lookup[4] = {B01000, B00100, B00010, B00001};
//------------ end motor definition

//++++++++++++ begin inducite cookplaat
//use relay to control de cookplate
int pinCookingPlateOnOff = 22;
int pinCookingPlateMode=24;
int pinCookingPlatePowerPlus=26;
int pinCookingPlatePowerMin=28;
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
int stopPinMoveOil=7;
int pinOil=34;
int pinLaoChou=36;
int pinShenChou =38;
int pinWater =40;
int pinJiu =42;
int pinMaYou =44;
int pinQianFeng =46;
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
int pinBusyLed=30;
int pinIdleLed=32;
//----- end pin definition

//////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  //use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
  pinMode(turnPinTotal, INPUT_PULLUP);
  pinMode(stopPinTotal, INPUT_PULLUP);  
  
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
  
  //liquid pomp
  pinMode(pinOil, OUTPUT); 
  digitalWrite(pinOil, HIGH);
  digitalWrite(stopPinMoveOil, INPUT_PULLUP);
  
   //initialize lcd
  LCDA.Initialise(); // 屏幕初始化
  delay(100);  

  currentPage=PAGE_MAIN;
  currentIndex=0;
  displayMainMenu();
  Serial.begin(9600); 
  Serial2.begin(9600); //for tts communication
    
  //------ region set motor begin state
   //set turn motor salt to start position
   // shiftMotor.goSteps(300, false, 0, 1, 2, 3, 1, motorSpeed28, turnPinSalt);
   // shiftMotor.sleepMotor();
   // shiftMotor.goSteps(-300, false, 0, 1, 2, 3, 1, motorSpeed28, turnPinSalt);
   // shiftMotor.sleepMotor();
    
       //MultiMotorDriver motorDriver= MultiMotorDriver(A1, A2, A0, 2, false, motorSpeed28);
       
       MultiStepperMotorDriver motorDriver= MultiStepperMotorDriver(A4, A5, A3, 2, false, motorSpeed28);
       
       Motor motors[3];
       Motor motorSalt = Motor(2, false, false, NO_STOP_PIN, -1512);  
       Motor motorSugar = Motor(2, true, false, NO_STOP_PIN, 512);  
       Motor motorWeijing = Motor(1, false, false, NO_STOP_PIN, 512);     
	////motorSalt.registerNr=1;
	////motorSalt.steps=1500;
	////motorSalt.id=1; //not neccessary, but easy for debug
	////motorSalt.firstOrSecondBlock=true;
	////motorSalt.isPin2and3Reversed=false;
	motors[0]=motorSalt;
        motors[1]=motorSugar;
        motors[2]=motorWeijing;
	motorDriver.driveMotors(motors, 3);

        delay(5000);
        
        //testStepMotors();
        
    //total motor to start position. direction is clockwise if face the as.
    //shiftMotor.goSteps(500, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin); //stopPinTotal=6;
    //shiftMotor.sleepMotor();
    //delay(1000);
    
    //total motor to start position. direction is clockwise if face the as.
    //shiftMotor.goSteps(500, false, 0, 1, 2, 3, 1, motorSpeed28, turnPinTotal); //stopPinTotal=6;
    //shiftMotor.sleepMotor();
    //delay(1000);
    
     //set final turn motor to start position. direction is clockwise if face the as.
    shiftMotor.goSteps(500, false, 4, 5, 6, 7, 2, motorSpeed28, stopPinTotal); //turnPinTotal=5;
    shiftMotor.sleepMotor();
    delay(1000);  
    
    //set move oil to start position
    //shiftMotor.goSteps(4500, true, 4, 6, 5, 7, 1, motorSpeed42Forward, stopPinMoveOil);  
    
    //shift motor2
    shiftMotor2.goSteps(500, false, 4, 5, 6, 7, 1, motorSpeed28, noStopPin);
    shiftMotor2.sleepMotor();
    delay(1000);  
    //
    
  //------ end region set motor begin state
  
   //moveTotal();  
   
   //takeSalt();
  
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
           carryOutCommands(commands[1]);
           break;
       }      
     }
     else if(currentPage==PAGE_CONFIG)
     {
       switch(currentIndex)
       {
         case 0:
           currentPage=PAGE_MAIN;
           currentIndex=0;
           displayLcd(currentPage, currentIndex);
           break;  
        case 1:
           displayBusyLcd();
           connectRelayPin(pinOil, 5000);
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 2:
           displayBusyLcd();
           moveTotal();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 3: //turn salt
           displayBusyLcd();
           doTakeSalt(660);
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 4: //move oil
           displayBusyLcd();
           moveOil();
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           break;
        case 5: //measure temprature
           LCDA.CLEAR();
           displayLineInLcd(0, ">back");    
           char output[16];       
           for(int icount=0; icount<10; icount++)
           {
             dtostrf(mlx.readAmbientTempC(),2,2,output);          
             Serial.print("Ambient = "); Serial.print(output);
             displayLineInLcd(1,  output);      
            
             dtostrf(mlx.readObjectTempC(),2,2,output);  
             displayLineInLcd(2,  output); 
             delay(500); 
           }
           currentPage=PAGE_CONFIG_DONE;
           displayDoneLcd();
           //currentPage=PAGE_MEASURE_TEMPRATURE;
           //delay(500);          
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

void moveOil(){   
    
   //positive clockwise   
   //660 is one turn(measured by practice)
    shiftMotor.goSteps(4500, true, 4, 6, 5, 7, 1, motorSpeed42Forward, stopPinMoveOil);
    shiftMotor.sleepMotor();
   // shiftMotor.goSteps(-1240, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
   // shiftMotor.sleepMotor();
   
    shiftMotor.goSteps(-3500, true, 4, 6, 5, 7, 1, motorSpeed42Forward, noStopPin);
    shiftMotor.sleepMotor();  
    delay(3000);   
    shiftMotor.goSteps(4500, true, 4, 6, 5, 7, 1, motorSpeed42Forward, stopPinMoveOil);
    shiftMotor.sleepMotor();
    //negative anticclockwise    
   // shiftMotor.goSteps(-200, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
   // shiftMotor.sleepMotor();
   // delay(5000); 
    
    //moveTotal();
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
