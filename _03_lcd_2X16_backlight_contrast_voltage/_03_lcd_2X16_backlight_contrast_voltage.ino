
////////////////////////////////////////////////
//
// This code may be freely used and copied.
//
// Gareth Davies - June 2012
//
////////////////////////////////////////////////

#include <LiquidCrystal.h>

//first connect backlight pin to potential meter, connect contrast pin to potential meter
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Select pins for LCD module
int pin_background=A0;
int pin_contrast=A1;

int valueA0;
int valueA1;


void setup()
{   
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("hello");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("     ");
}

void loop()
{   
  delay(500);
   valueA0=analogRead(pin_background);
   lcd.setCursor(0, 0);
   lcd.print("backgr");
   lcd.setCursor(7, 0);
   lcd.print("     ");
   lcd.setCursor(7, 0);
   lcd.print(valueA0);
   
   
   valueA1=analogRead(pin_contrast);
   lcd.setCursor(0, 1);
   lcd.print("contra");
   lcd.setCursor(7, 1);
   lcd.print("     ");
   lcd.setCursor(7, 1);
   lcd.print(valueA1);
}
