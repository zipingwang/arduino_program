int pinSignalOutput=A2;
int pinLed=7;

void setup(){
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  
  digitalWrite(pinLed, LOW);
  
   //interrupt 5(pin 18 in arduino mega)
   //pinMode(18, INPUT_PULLUP);
   //attachInterrupt(5, userHandleRequest, CHANGE);
   
}

void loop(){

  int SignalOutput = analogRead(pinSignalOutput);  //not push button 647, when push button value is about 20
  int binaryValue=digitalRead(pinSignalOutput);
  
  //if(SignalOutput<600)
  //  digitalWrite(pinLed, HIGH);
  digitalWrite(pinLed, binaryValue);
  if(binaryValue==HIGH)
    delay(2000);  //show led
  
  Serial.println(SignalOutput); 
  Serial.println(binaryValue);
  
  delay(100); //just here to slow down the output for easier reading
  
}

void userHandleRequest()
{
   int binaryValue=digitalRead(pinSignalOutput);
   //Serial.println(binaryValue);
   digitalWrite(pinLed, binaryValue);
   delay(1000);
}
