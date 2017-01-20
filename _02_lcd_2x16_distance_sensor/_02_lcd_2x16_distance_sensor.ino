/*
material: lcd 2x16, HC-SR04 distance sensor.
schema: _02_lcd_2x16_distance_sensor.fzz
http://4tronix.co.uk/arduino/Ultra-Sonic.php
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int pingPin = 7; //output 10us pulse to this pin //connect to trig pin in HC-SR04
int inPin = 6; //measure return pulse width from this pin //connect to Echo pin in HC-SR04
long duration, inches, cm;
int indec, cmdec;
int inchconv = 147; // ratio between puls width and inches
int cmconv = 59; // ratio between pulse width and cm
String s1, s2;

// initialise LCD library and pins
void setup() {
  lcd.begin(16, 2);
  pinMode(pingPin, OUTPUT);
  pinMode(inPin, INPUT);
}

void loop()
{
  // Send a short LOW followed by HIGH pulse to Trigger input:
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  // read the length of the return pulse on Echo output
  duration = pulseIn(inPin, HIGH);

  // convert the time into a distance (non-floating point with decimals
  inches = microsecondsToInches(duration);
  indec = (duration - inches * inchconv) * 10 / inchconv;
  cm = microsecondsToCentimeters(duration);
  cmdec = (duration - cm * cmconv) * 10 / cmconv;
  s1 = String(inches) + "." + String(indec) + "in" + "     ";
  s2 = String(cm) + "." + String(cmdec) + "cm" + "     ";
  lcd.setCursor(0, 0); // print inches on top line of LCD
  lcd.print(s1);
  lcd.setCursor(0,1); // print cm on second line of LCD
  lcd.print(s2);

  delay(100);
}

long microsecondsToInches(long microseconds)
{
  return microseconds / inchconv;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / cmconv;
}



