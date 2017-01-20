int pinSignalOutput=A2;
int pinLed=7;
bool pinBaowenPresssed=false;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay =1000;    // the debounce time; increase if the output flickers


void setup(){
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  
  digitalWrite(pinLed, LOW);
  
   //interrupt 5(pin 18 in arduino mega)
   pinMode(18, INPUT_PULLUP);
   attachInterrupt(5, userHandleRequest, CHANGE);   
}

void loop(){
  if(pinBaowenPresssed)
      digitalWrite(pinLed, HIGH); 
   else 
     digitalWrite(pinLed, LOW); 
}

//when dianfanguo baowen button pressed, analog value will be 647>1024/5, so it will be 1 when digitalRead, and will cause interrupt(change) happen.
void userHandleRequest()
{
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    pinBaowenPresssed=!pinBaowenPresssed;
    lastDebounceTime=millis();
  }
}
