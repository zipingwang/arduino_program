#include "LCD12864RSPI.h"
#include <Keypad.h>
/*
this LCD  Arduino example use SPI(serial mode)
PIN1 GND = GND
PIN2 VCC = 5V
//if want to use different port for RS, RW and EN. change LCD12864RSPI.h and arduino sketch code.
Pin4 RS(CS) latchpin = 31; 
Pin5 RW(SID) datapin= 32; 
Pin 6 EN(CLK) clockpin= 35;
PIN15 PSB = GND;  //serial mode
有乱码，有时不能正确显示，set RST(Pin 17) LOW.
Pin 19 +5;
Pin 20 GND;
*/
 
//#include "LCD12864RSPI.h"
#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )
 
unsigned char show0[]={0xBC,0xAB,0xBF,0xCD,0xB9,0xA4,0xB7,0xBB};//极客工坊
unsigned char show1[]="geek-workshop";
unsigned char logo0[]={                                         //笑脸
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,
      0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,
      0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
      0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x03,0xFE,0x00,
      0x7F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,
      0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,
      0x01,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x00,
      0x00,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
      0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x01,0xF0,0x00,0x00,
      0x00,0x0F,0x80,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x03,0xE0,0x00,0x00,
      0x00,0x07,0xC0,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,
      0x00,0x01,0xE0,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x07,0x80,0x00,0x00,
      0x00,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,
      0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x1E,0x00,0x00,0x00,
      0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x3C,0x00,0x00,0x00,
      0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x38,0x00,0x40,0x00,
      0x00,0x80,0x1C,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x78,0x03,0xF8,0x00,
      0x07,0xF0,0x1E,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x70,0x07,0xFE,0x00,
      0x0F,0xFC,0x0E,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0xF0,0x1F,0xBF,0x00,
      0x3F,0x7E,0x0F,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0xE0,0x3E,0x0F,0x80,
      0x7C,0x1F,0x07,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0xE0,0x7C,0x07,0xC0,
      0xF8,0x0F,0x87,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x01,0xE0,0xF8,0x01,0xE1,
      0xF0,0x03,0xC3,0x80,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x01,0xC0,0xF0,0x01,0xE1,
      0xE0,0x03,0xC3,0x80,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x01,0xC0,0xE0,0x00,0xE1,
      0xC0,0x01,0xC3,0x80,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x01,0xC0,0x00,0x00,0x00,
      0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
      0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x01,0xC0,0x00,0x00,0x00,
      0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x01,0xC0,0x00,0x00,0x00,
      0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x01,0xC0,0x00,0x00,0x00,
      0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x01,0xE0,0x00,0x00,0x00,
      0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,
      0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,
      0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0xF0,0x00,0x0C,0x00,
      0x0C,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x70,0x00,0x1E,0x00,
      0x1E,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x78,0x00,0x1F,0x00,
      0x3E,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x38,0x00,0x07,0x80,
      0xF8,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x3C,0x00,0x03,0xE1,
      0xF0,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x1E,0x00,0x01,0xFF,
      0xE0,0x00,0x78,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0xFF,
      0x80,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x3F,
      0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x07,0x80,0x00,0x00,
      0x00,0x01,0xE0,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x03,0xE0,0x00,0x00,
      0x00,0x07,0xC0,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x01,0xF0,0x00,0x00,
      0x00,0x0F,0x80,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
      0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x00,
      0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,
      0x01,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x0F,0xE0,0x00,
      0x07,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x03,0xFE,0x00,
      0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
      0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,
      0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,
      0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
 

 unsigned char jiChaoFan[]={
  0xBC, 0xA6,
  0xB3, 0xB4,
  0xB7, 0xB9
     };                    //鸡炒饭
     
unsigned char gaLiJi[]={
  0xBF, 0xA7,
  0xE0, 0xAC,
  0xBC, 0xA6
     };                    //咖喱鸡

unsigned char pleaseInputNumber[]={
  0xC7, 0xEB,
  0xCA, 0xE4,
  0xC8, 0xEB,
  0xBA, 0xC5,
  0xC2, 0xEB,
  0xA3, 0xBA
     };                    //请输入号码：
     
  unsigned char AShuRuHaoMa[]={
  0x41, 0x00,
  0xA1, 0xA1,
  0xCA, 0xE4,
  0xC8, 0xEB,
  0xBA, 0xC5,
  0xC2, 0xEB
     };                    //A　输入号码


//keypad
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

//end keypad
void setup()
{ 
  LCDA.Initialise(); // 屏幕初始化
  delay(100);
  Serial.begin(9600);  
  LCDA.DisplayString(0,0,AShuRuHaoMa,AR_SIZE(AShuRuHaoMa));//第一行第1格开始
}
 
 boolean beginInput=false;
 int inputNumber=0;
 int charLength=0;
void loop()
{
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case 'A':
         beginInput=true;
         LCDA.CLEAR();//清屏
         delay(200);
         LCDA.DisplayString(0,0,pleaseInputNumber,AR_SIZE(pleaseInputNumber));//第一行第1格开始，显示文字极客工坊
        break;
      case 'D':
        if(beginInput==true)
        {
           switch (inputNumber)
           {
             case 1:
                LCDA.CLEAR();//清屏
                delay(200);
                LCDA.DisplayString(0,2,jiChaoFan,AR_SIZE(jiChaoFan));//第一行第三格开始，显示文字极客工坊
                break;
             case 12:
               LCDA.CLEAR();//清屏
                LCDA.DisplayString(0,2,gaLiJi,AR_SIZE(gaLiJi));//第一行第三格开始，显示文字极客工坊
                break;
           }
           inputNumber=0;
           charLength=0;
        }
        beginInput=false;
        break;
      case '*':
        LCDA.CLEAR();//清屏
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
        if(beginInput==true)
        {
          charLength+=1;
          
          inputNumber=inputNumber*10+int(key)-48;
          
          //LCDA.CLEAR();//清屏
        //delay(200);
         
         //LCDA.DisplayString(0,2,pleaseInputNumber,AR_SIZE(pleaseInputNumber));//第一行第三格开始，显示文字极客工坊
         
         //char inputChar[1];
         //char * pInputChar=inputChar;
         //sprintf((char *)inputChar, "%d", inputNumber);
        // pInputChar=itoa(inputNumber, inputChar, 10);
         //int temp=0;
         //for(int i=0; i<charLength; i++)
         //{
         //  inputChar[i]=inputNumber/
         //}      
        
          LCDA.DisplaySig(1,2 + charLength, key);//第一行第三格开始，显示文字极客工坊 
        }
      default:
        //LCDA.CLEAR();//清屏
        //LCDA.DisplayString(0,2, key);//第一行第三格开始，显示文字极客工坊
        Serial.println(key);
    }
  }
}
