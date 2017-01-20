
//http://forum.pjrc.com/threads/23763-28BYJ-48-stepper-motor-works

int stepper28Pin1=4;
int stepper28Pin2=5;
int stepper28Pin3=6; 
int stepper28Pin4=7;

//boolean flag=false;

//use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
int turnPin=12;
int huataiStopPin=3;



int huataiPin1 = 8;   // A+
int huataiPin2 =10;   // B+
int huataiPin3 =9;    // A-
int huataiPin4 = 11;  // B-                       

//int huataiSpeed =2000; //for 4 steps: minimun 1500, how smaller the number,how faster. 16385 is the maximun. then restart, e.g 18385=2000
 //for 8 steps: minimun 600, how smaller the number,how faster. 16385 is the maximun. then restart, e.g 18385=2000. 5000 could be the best
 
int count = 0;          // count of steps made
int countsperrev =150;// how many rotation, how long distance it will move.

//int steps=4;
//int lookup[4] = {B01000, B00100, B00010, B00001};

int turnDirection=-1;
//int halfTurnStepCount=17; //one step is 17

int motorSpeed28 =1000; //1200;  //variable to set stepper speed
int motorSpeedHuatai =1000; //1200;  //variable to set stepper speed
//int count = 0;          // count of steps made
//int countsperrev =1250;// 512; // number of steps per full revolution
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
//int lookup[4] = {B01000, B00100, B00010, B00001};
int steps=8;

void setup() {
  
  //declare the motor pins as outputs
  pinMode(huataiPin1, OUTPUT);
  pinMode(huataiPin2, OUTPUT);
  pinMode(huataiPin3, OUTPUT);
  pinMode(huataiPin4, OUTPUT);
  
  pinMode(stepper28Pin1, OUTPUT);
  pinMode(stepper28Pin2, OUTPUT);
  pinMode(stepper28Pin3, OUTPUT);
  pinMode(stepper28Pin4, OUTPUT);

   //use internal pull up register http://arduino.cc/en/Tutorial/InputPullupSerial
   pinMode(turnPin, INPUT_PULLUP);
   pinMode(huataiStopPin, INPUT_PULLUP);
   
    // initialize the serial port:
    Serial.begin(9600);
  
    setStepper28ToStartPosition();
    delay(2000);
    /*
    int icountTemp=0;
     myStepper.step(-stepsPerRevolution*10);
    for(int icount=0; icount<stepsPerRevolution*200; icount+=stepsPerRevolution)
    {
      myStepper.step(-stepsPerRevolution);
      icountTemp++;
      if(digitalRead(turnPin)==LOW)
        break;      
    }
    Serial.println("icountTemp");
     Serial.println(icountTemp);
    halfTurnStepCount=icountTemp/2 +10/2 +1;
    */
   // halfTurnStepCount=23;
    /*
     myStepper.step(320);
       for(int icount=0; icount<stepsPerRevolution*320; icount+=32)
    {
      if(digitalRead(turnPin)==LOW)
        break;
      myStepper.step(32);
    }
    */
    moveForward();
    //myStepper.step(-stepsPerRevolution*18); //64*32 is 360 degree.
    delay(2000);
}

void loop() {
 
  moveBack();
  stepper28HalfTurn();
  moveForward();
   delay(2000);  
  setStepper28ToStartPosition();
   delay(2000);
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)
void anticlockwise()
{
  for(int i = 0; i < steps; i++)
  // for(int i = 0; i < 4; i++)
  {
    setOutputHuatai(i);
    delayMicroseconds(motorSpeedHuatai);
  }
}

void clockwise()
{
  for(int i = steps-1; i >= 0; i--)
  // for(int i = 3; i >= 0; i--)
  {
    setOutputHuatai(i);
    delayMicroseconds(motorSpeedHuatai);
  }
}



void moveBack()
{
 //move back
   count=0;
  while(count < countsperrev )
  {
    
    clockwise();
    count++;
  }
}


void moveForward()
{
 //move back
   count=0;
  while(count < countsperrev*2 )
  {
    if(digitalRead(huataiStopPin)==LOW)
        break;
    anticlockwise();
    count++;
  }
}
void halfTurn()
{  
   for(int j=0; j<500; j++)
  {         
       for(int i = 0; i <steps; i++)
      // for(int i = 3; i >= 0; i--)
      {
         
            
        setOutput28(i);
        delayMicroseconds(motorSpeed28);
      }
  }
}
void setStepper28ToStartPosition()
{
  for(int j=0; j<1000; j++)
  {         
     
         for(int i = steps-1; i >= 0; i--)
      {
         if(digitalRead(turnPin)==LOW)
         {
           j=1000;
            break;
         }
            
        setOutput28(i);
        delayMicroseconds(motorSpeed28);
      }
  }
}

void stepper28HalfTurn()
{
   for(int j=0; j<300; j++)
  {         
       for(int i = 0; i <steps; i++)
      // for(int i = 3; i >= 0; i--)
      {
         
            
        setOutput28(i);
        delayMicroseconds(motorSpeed28);
      }
  }
}

void setOutput28(int out)
{
  digitalWrite(stepper28Pin1, bitRead(lookup[out], 0));
  digitalWrite(stepper28Pin2, bitRead(lookup[out], 1));
  digitalWrite(stepper28Pin3, bitRead(lookup[out], 2));
  digitalWrite(stepper28Pin4, bitRead(lookup[out], 3));
}

void setOutputHuatai(int out)
{
  digitalWrite(huataiPin1, bitRead(lookup[out], 0));
  digitalWrite(huataiPin2, bitRead(lookup[out], 1));
  digitalWrite(huataiPin3, bitRead(lookup[out], 2));
  digitalWrite(huataiPin4, bitRead(lookup[out], 3));
}
