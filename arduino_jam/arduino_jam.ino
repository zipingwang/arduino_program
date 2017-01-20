
////////////////////////////////////////////////
// This demo code reads the codes from an IR remote
// and displays them on a 2-line LCD
//
// This code may be freely used and copied.
//
// Gareth Davies - June 2012
//
////////////////////////////////////////////////

#include <LiquidCrystal.h>
#include <Servo.h> 

#define IR_IN  8  // Use ditial pin 8 for input from receiver
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Select pins for LCD module
int command=0;
int Pulse_Width=0;
int ir_code=0x00;
char  adrL_code=0x00;
char  adrH_code=0x00;

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

void timer1_init(void)
{
  
  TCCR1A = 0X00; 
  TCCR1B = 0X05;
  TCCR1C = 0X00;
  TCNT1 = 0X00;
  TIMSK1 = 0X00;
}

void remote_deal(void)
{ 
  doCommand1();
    //lcd.clear();
    //delay(100);
    //lcd.setCursor(0,0);
    //lcd.print(ir_code,HEX);
    //lcd.setCursor(0,1);
    //lcd.print(adrL_code,HEX);
    if(adrL_code==0x16)
    {
      command=0;
      Serial.println("0");
    }
    else if(adrL_code==0x0c)
    {
      if(command!=1)
      {
        doCommand1();
      }
        command=1;    
        Serial.println("1");
    }
    else if(adrL_code==0x18)
    {
       command=2;
       Serial.println("2");
    }
    else if(adrL_code==0x5E)
    {
      command=3;
        Serial.println("3");
    }
   // Serial.println(ir_code,HEX);
   // Serial.println(adrL_code,HEX);
}

char logic_value()
{
  TCNT1 = 0X00;
  while(!(digitalRead(8)));
  Pulse_Width=TCNT1;
  TCNT1=0;
  if(Pulse_Width>=7&&Pulse_Width<=10)
  {
    while(digitalRead(8));
    Pulse_Width=TCNT1;
    TCNT1=0;
    if(Pulse_Width>=7&&Pulse_Width<=10) //560us
      return 0;
    else if(Pulse_Width>=25&&Pulse_Width<=27) //1.7ms
      return 1;
  }
  return -1;
}

void pulse_deal()
{
  int i;
  int j;
  ir_code=0x00;
  adrL_code=0x00;
  adrH_code=0x00;

  for(i = 0 ; i < 16; i++)
  {
    if(logic_value() == 1)
        ir_code |= (1<<i);
  }
  for(i = 0 ; i < 8; i++)
  {
    if(logic_value() == 1)
      adrL_code |= (1<<i);
  }
  for(j = 0 ; j < 8; j++)
  {
    if(logic_value() == 1)
        adrH_code |= (1<<j);
  }
}

void remote_decode(void)
{
  TCNT1=0X00;       
  while(digitalRead(8))
  {
    if(TCNT1>=1563)
    {
      ir_code=0x00ff;
      adrL_code=0x00;
      adrH_code=0x00;
      return;
    }  
  }
  TCNT1=0X00;
  while(!(digitalRead(8)));
  Pulse_Width=TCNT1;
  TCNT1=0;
  if(Pulse_Width>=140&&Pulse_Width<=141) //9ms
  {

    while(digitalRead(8));
    Pulse_Width=TCNT1;
    TCNT1=0;
    if(Pulse_Width>=68&&Pulse_Width<=72) //4.5ms
    {  
      pulse_deal();
      return;
    }
    else if(Pulse_Width>=34&&Pulse_Width<=36) //2.25ms
    {
      while(!(digitalRead(8)));
      Pulse_Width=TCNT1;
      TCNT1=0;
      if(Pulse_Width>=7&&Pulse_Width<=10) //560us
      {
        return; 
      }
    }
  }
}

void setup()
{
  unsigned char i;
  pinMode(IR_IN,INPUT);
  myservo.attach(11);
  //lcd.begin(16, 2);
   Serial.begin(9600);
}

void loop()
{  
  /*
  while(1)
  {
    doCommand1();
  }
  */
  
  timer1_init();
  while(1)
  {
    remote_decode();
    remote_deal();   
  } 
  
  
}

void doCommand1()
{
   Serial.println("docommand1");
   myservo.writeMicroseconds(1500);              // tell servo to go to position in variable 'pos' 
   delay(1500);    
   myservo.writeMicroseconds(1000);              // tell servo to go to position in variable 'pos' 
   delay(1000);
  
   Serial.println("end docommand1");
   /*
   Serial.println("docommand1");
   myservo.write(30);              // tell servo to go to position in variable 'pos' 
   delay(600); 
   myservo.write(0); 
   delay(1000); 
   */
}

void doCommand2()
{
   myservo.write(60);              // tell servo to go to position in variable 'pos' 
   delay(1000); 
   myservo.write(0); 
   delay(1000); 
}
void doCommand3()
{
   myservo.write(90);              // tell servo to go to position in variable 'pos' 
   delay(3200); 
   myservo.write(0); 
   delay(1000); 
    myservo.write(60);              // tell servo to go to position in variable 'pos' 
   delay(1200); 
   myservo.write(0); 
   delay(1000); 
}
