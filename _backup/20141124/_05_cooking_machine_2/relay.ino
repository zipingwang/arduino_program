void connectRelayPin(int pin)
{
   digitalWrite(pin, LOW);   // low is working, connect. relay 是低电平有效,常开模式
   //delay(200);               // wait 200 ms time
   //digitalWrite(pin, HIGH);   // high is disconnct
   //delay(200);     
}

void connectRelayPin(int pin, int durationMilSecond)
{
   digitalWrite(pin, LOW);   // low is working, connect. relay 是低电平有效,常开模式
   delay(durationMilSecond);               // wait x time
   //digitalWrite(pin, HIGH);   // high is disconnct
   //delay(200);     
}

void disconnectRelayPin(int pin)
{
   //digitalWrite(pin, );   // low is working, connect. relay 是低电平有效,常开模式
   //delay(200);               // wait 200 ms time
   digitalWrite(pin, HIGH);   // high is disconnct
   //delay(200);     
}

void disconnectRelayPin(int pin, int durationMilSecond)
{
   //digitalWrite(pin, LOW);   // low is working, connect. relay 是低电平有效,常开模式
   //delay(durationMilSecond);               // wait x time
   digitalWrite(pin, HIGH);   // high is disconnct
   delay(200);     
}

void touchCookingPlateWire(int pin)
{
  connectRelayPin(pin);
  delay(200);
  disconnectRelayPin(pin);
  delay(200);
}
