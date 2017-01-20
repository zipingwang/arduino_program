#include "dish.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "Max6675.h"
#include <SoftwareSerial.h>
 


#include <OneWire.h>
#include <DallasTemperature.h>

byte END_BYTE=127; //java max byte is 127, from android send byte max value is 127
//int END_BYTE='\r';
byte buffer[BUFFER_SIZE];

//++++++++++++ begin inducite cookplaat
//use relay to control de cookplate
int pinCookingPlateOnOff = A0;
int pinCookingPlateMode=A1; 
int pinCookingPlatePowerPlus=A2; 
int pinCookingPlatePowerMin=A3; 
int pinCookingPlateMaximum=A4;
int pinCookingPlateMinimum=A5;
//------------ end inducite cookplaat



#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )
unsigned char cookingEggMsg[]="busy...";
unsigned char cookingSpaghettiMsg[]="busy...";
unsigned char cookingDone[]="Done.";
//--------------- end lcd difinition


//++++ begin mlx90614
//connect pin20 to SDA, pin 21 to SCL for arduino mega
// Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 //----- end mlx
 
//+++++ begin pin definition
//int pinBusyLed=53; //if use relay it is 30
//int pinIdleLed=51; //if use relay it is 32
//----- end pin definition

//temperature sensor MAX6675// Max6675 module: SO on pin #8, SS on pin #9, CSK on pin #10 of Arduino UNO
// Other pins are capable to run this library, as long as digitalRead works on SO,
// and digitalWrite works on SS and CSK

//Max6675 ts(8, 9, 10);

//temperature sensor DS18x20
int pinDS1820=12;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(pinDS1820);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature ds1820(&oneWire);



char output[16]; //buffer for temperator
double objectT=0;
float temperature;

int teller=0;

SoftwareSerial bluetooth(10, 11); // TX, RX
//////////////////////////////////////////////////////////////////////////////

void setup() {   
  Serial.begin(9600);
  
  //The Arduino Mega has three additional serial ports:  on pins 19 (RX) and 18 (TX), Serial2 on pins 17 (RX) and 16 (TX), Serial3 on pins 15 (RX) and 14 (TX). 
  bluetooth.begin(57600); //set baud rate default is 9600 for HC-06, see _029_bluetooth_change_baudrate how to change baud rate
  
  //cooking plate
  pinMode(pinCookingPlateOnOff, OUTPUT);  
  pinMode(pinCookingPlateMode, OUTPUT);  
  pinMode(pinCookingPlatePowerPlus, OUTPUT);  
  pinMode(pinCookingPlatePowerMin, OUTPUT);  
  pinMode(pinCookingPlateMaximum, OUTPUT);  
  pinMode(pinCookingPlateMinimum, OUTPUT);  
  
  //realy is 低电平有效
  digitalWrite(pinCookingPlateOnOff, HIGH);
  digitalWrite(pinCookingPlateMode, HIGH);
  digitalWrite(pinCookingPlatePowerPlus, HIGH);
  digitalWrite(pinCookingPlatePowerMin, HIGH);
  //digitalWrite(pinCookingPlateMaximum, HIGH);
  digitalWrite(pinCookingPlateMinimum, HIGH);
  
  //+++++ begin pin definition
  //pinMode(pinBusyLed, OUTPUT); 
  //digitalWrite(pinBusyLed, HIGH); //if it goes through relay, relay low has effect
 // digitalWrite(pinBusyLed, LOW);
 // pinMode(pinIdleLed, OUTPUT); 
  //digitalWrite(pinIdleLed, HIGH); //if it goes through relay, relay low has effect
//  digitalWrite(pinIdleLed, LOW);
 //----- end pin definition
 
 //MLX90614
//  mlx.begin(); 
  
  //temperature sensor
//  ts.setOffset(10);
  // set offset for temperature measurement.
  // 1 stannds for -0.25 Celsius
  
  //pinMode(pinDS1820, INPUT_PULLUP);
   // Start up the ds1820
  ds1820.begin();
}

 void loop()
{         
   //reportTemperature();
         
  if (readline(bluetooth.read(), buffer, BUFFER_SIZE) > 0) {
    Serial.write("first byte is:");
    Serial.println(buffer[0]);
    
    if(buffer[0]==(byte)'a')
    {      
       bluetooth.print("carry out command a");     
     //carryOutCommands(commands[buffer[1]-(byte)'0']);     
      bluetooth.write((byte)END_BYTE);
      carryOutCommands(commands[buffer[1]]);
    }
    else if (buffer[0]==(byte)'b')
    {    
      // bluetoothbluetooth.write("carry out command b dfsdf sdfsi 123456 abce efg hij k lmn opq rst uvw xyz hello this is end");
       
      if(buffer[1]==1)
      {      
         doTurnOnCookingPlate();  
      }
      else if(buffer[1]==2)
      {      
         doSwitchCookingPlateToMode();  
      }
      else if(buffer[1]==3)
      {      
         doSetPlatePowerPlus(1);  
      }
      else if(buffer[1]==4)
      {      
         doSetPlatePowerMin(1);  
      }
       else if(buffer[1]==5)
      {      
         doSetPlatePowerPlus(10);  
      }
       else if(buffer[1]==6)
      {      
         doSetPlatePowerMin(10);  
      }
       else if(buffer[1]==7)
      {      
         reportTemperature(); 
      }
    }
    else if (buffer[0]==(byte)'c')
    {
      unsigned char commandc[BUFFER_SIZE]={'A',1,'A',2,'A',3, 4,'F',1,10,'A',4,'ÿ'}; //must use BUFFER_SIZE
      commandc[9]= buffer[1];
      carryOutCommands(commandc);
    }
    else if (buffer[0]==(byte)'d')
    {
      unsigned char commandd[BUFFER_SIZE]={'A',1,'A',2,'A',3, 4,'O',1,90,'F',1,0,'A',4,'ÿ'};
      commandd[9]=buffer[1];
      commandd[12]=buffer[2];
      carryOutCommands(commandd);
    }
    //  bluetooth.print((byte)END_BYTE); android receive as 127 three bytes 49 50 55
   // bluetooth.write((byte)END_BYTE);
      
    
  }
}
    
static int pos = 0;
int readline(int readch, byte *buffer, int len)
{   
   int rpos;
   if (readch >= 0) {
     //Serial.write(readch);
     if (pos < len-1) {
         buffer[pos] = readch;  
         pos++;  
         if(readch==END_BYTE)
         {
           rpos = pos;
           //Serial.write("end");
           pos=0;
           return rpos;
         }
     }
   }
  // No end of line has been found, so return -1.
  return -1;
}

boolean isTemperatureReached(double t, double tlimit)
{
  if(tlimit<0)
    return false;
  
  if(t>tlimit)
    return true;
  else
    return false;
}

void reportMessage(char * msg)
{
   bluetooth.write(msg);       
   bluetooth.write((byte)END_BYTE);      //bluetooth.flush();
}

void reportTemperature()
  {
    reportTemperature(-1);
  }

bool reportTemperature(double tlimit, double elapsedTime)
{
  teller++;
  if(elapsedTime>=0)
  {
    bluetooth.write("wanted T:");   
    dtostrf(tlimit,2,0,output);  
    bluetooth.write(output);
    bluetooth.write(" elapsed:");   
    dtostrf(elapsedTime,3,0,output);  
    bluetooth.write(output); 
    bluetooth.write("s   "); 
  }
  
  //ds1820
    ds1820.requestTemperatures(); // Send the command to get temperatures  
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  bluetooth.write("T: ");
  temperature=ds1820.getTempCByIndex(0);
  dtostrf(temperature,2,2,output);  
  bluetooth.write(output); 
  bluetooth.write("    ");
  bluetooth.print(teller);
  bluetooth.write((byte)END_BYTE);      //bluetooth.flush();
   if(isTemperatureReached(temperature, tlimit))
   {
     bluetooth.write("tpr");   //means temprature reached   //bluetooth.flush();
     dtostrf(temperature,2,2,output);  
     bluetooth.write(output); 
     bluetooth.write((byte)END_BYTE);      //bluetooth.flush();
     return true;
   }
   else
   {
     return false;
   }
}
bool reportTemperature(double tlimit)
{
  return reportTemperature(tlimit, -1);
}






 

