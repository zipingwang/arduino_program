// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


//http://arduino.cc/en/Tutorial/Sweep

#include <Servo.h> 
 
#define IR_IN  6  // Use ditial pin 8 for input from receiver
int Pulse_Width=0;
int ir_code=0x00;
char  adrL_code=0x00;
char  adrH_code=0x00;
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
int potPin0 = 0;
int potPin1 = 1;
int sprayDuration = 0;
int sprayStrengh=0;

void setup() 
{ 
  unsigned char i;
  pinMode(IR_IN,INPUT);
  
  myservo.attach(7);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);
} 
 
 
void loop() 
{ 
  timer1_init();
  remote_decode();
    remote_deal();
    
   sprayDuration = analogRead(potPin0);
   sprayStrengh = analogRead(potPin1);

   sprayDuration = map(sprayDuration, 0, 1023, 500, 2550);
    Serial.println("spary duration ");
      Serial.println(sprayDuration);
      // delay(550); 
     
   sprayStrengh = map(sprayStrengh, 0, 1023, 0, 90); 
   Serial.println("spary Strengh ");
   Serial.println(sprayStrengh);
   /*
   myservo.write(-90); 
   delay(1000);
   myservo.write(0); 
   delay(10000);
    //delay(1000);
    */
    
   myservo.write(sprayStrengh);              // tell servo to go to position in variable 'pos' 
   delay(sprayDuration); 
   myservo.write(0); 
   delay(1000); 
   
   /*
   int val=0;
     val = analogRead(potPin1);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  Serial.println(val);
  //myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(550);    
*/  
   /* 
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  */
} 


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
    //lcd.clear();
    //delay(1);
    //lcd.setCursor(0,0);
    //lcd.print(ir_code,HEX);
    //lcd.setCursor(0,1);
    //lcd.print(adrL_code,HEX);
    Serial.println(ir_code,HEX);
     Serial.println(adrL_code, HEX);
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
