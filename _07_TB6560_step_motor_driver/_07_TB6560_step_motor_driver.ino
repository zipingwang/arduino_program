// the setup routine runs once when you press reset:
int pinClkPlus=4; //pulse for step motor, high->give signal, low->no signal
int pinCW=5; // fro direction, clockwise or anti clockwise

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(pinClkPlus, OUTPUT);
  pinMode(pinCW, OUTPUT);
  digitalWrite(pinClkPlus, LOW);
  digitalWrite(pinCW, LOW);
}

void loop2() {

   digitalWrite(pinClkPlus, HIGH);
   delayMicroseconds(500);
   digitalWrite(pinClkPlus, LOW);
   delayMicroseconds(500);
 
}
// the loop routine runs over and over again forever:
void loop() {
 // Serial.write("h");
  digitalWrite(pinCW, LOW);
 for(int icount=0; icount<1000; icount++)
 {
   digitalWrite(pinClkPlus, HIGH);
   delayMicroseconds(1000);
   digitalWrite(pinClkPlus, LOW);
   delayMicroseconds(1000);
 }
 delay(3000);
 digitalWrite(pinCW, HIGH);
 for(int icount=0; icount<1000; icount++)
 {
   digitalWrite(pinClkPlus, HIGH);
   delayMicroseconds(1000);
   digitalWrite(pinClkPlus, LOW);
   delayMicroseconds(1000);
   
 }
 delay(3000);
}
