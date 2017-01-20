int pinSignalOutput=A2;
int pinLed=7;
bool started=false;

int SignalOutput=0;
void setup(){
  Serial.begin(115200);
  pinMode(pinLed, OUTPUT);
  
  digitalWrite(pinLed, LOW);
  
   //interrupt 5(pin 18 in arduino mega)
   //pinMode(18, INPUT_PULLUP);
   //attachInterrupt(5, userHandleRequest, CHANGE);
   
}

void loop(){
    while(started==false)
    {
      //SignalOutput = analogRead(pinSignalOutput);
      if(analogRead(pinSignalOutput)>=3)
        started=true;
      
      //delay(2);
    }
    byte data[100];
    int mean=0;
    for(int i=0; i<50; i++)
    {
      delay(2);
      mean+=analogRead(pinSignalOutput);
      //data[i] =constrain(analogRead(pinSignalOutput), 0, 255);  //not push button 647, when push button value is about 20
    }
    //int binaryValue=digitalRead(pinSignalOutput);
    
    //if(SignalOutput<600)
    //  digitalWrite(pinLed, HIGH);
    //digitalWrite(pinLed, binaryValue);
    //if(binaryValue==LOW) //button pushed
    //  delay(2000);  //led off for 2 secs
    //Serial.println(analogRead(pinSignalOutput));
    Serial.println(mean/50); 
   // Serial.println(binaryValue);
  
  //delay(100); //just here to slow down the output for easier reading
  
}

void userHandleRequest()
{
   int binaryValue=digitalRead(pinSignalOutput);  //problem with interrupt is that when read again it will get the new state value. sometime works sometime not.
   //Serial.println(binaryValue);
   digitalWrite(pinLed, binaryValue);
   delay(1000);
}
