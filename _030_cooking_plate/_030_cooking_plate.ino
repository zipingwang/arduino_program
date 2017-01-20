#include "dish.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "Max6675.h"

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
 Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 //----- end mlx
 
//+++++ begin pin definition
int pinBusyLed=53; //if use relay it is 30
int pinIdleLed=51; //if use relay it is 32
//----- end pin definition

//temperature sensor MAX6675// Max6675 module: SO on pin #8, SS on pin #9, CSK on pin #10 of Arduino UNO
// Other pins are capable to run this library, as long as digitalRead works on SO,
// and digitalWrite works on SS and CSK

Max6675 ts(8, 9, 10);

//temperature sensor DS18x20
int pinDS1820=15;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(pinDS1820);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature ds1820(&oneWire);



char output[16]; //buffer for temperator
double objectT=0;
float temperature;
//////////////////////////////////////////////////////////////////////////////

void setup() {   
  Serial.begin(9600);
  
  //The Arduino Mega has three additional serial ports: Serial1 on pins 19 (RX) and 18 (TX), Serial2 on pins 17 (RX) and 16 (TX), Serial3 on pins 15 (RX) and 14 (TX). 
  Serial1.begin(57600); //set baud rate default is 9600 for HC-06, see _029_bluetooth_change_baudrate how to change baud rate
  
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
  digitalWrite(pinCookingPlateMaximum, HIGH);
  digitalWrite(pinCookingPlateMinimum, HIGH);
  
  //+++++ begin pin definition
  pinMode(pinBusyLed, OUTPUT); 
  //digitalWrite(pinBusyLed, HIGH); //if it goes through relay, relay low has effect
  digitalWrite(pinBusyLed, LOW);
  pinMode(pinIdleLed, OUTPUT); 
  //digitalWrite(pinIdleLed, HIGH); //if it goes through relay, relay low has effect
  digitalWrite(pinIdleLed, LOW);
 //----- end pin definition
 
 //MLX90614
  mlx.begin(); 
  
  //temperature sensor
  ts.setOffset(10);
  // set offset for temperature measurement.
  // 1 stannds for -0.25 Celsius
  
  //pinMode(pinDS1820, INPUT_PULLUP);
   // Start up the ds1820
  ds1820.begin();
}

 void loop()
{
  /*
  //NB: if you are using an arduino mega change this line of code while(Serial.available()); to while(Serial.available()>0); otherwise the code will not work
  while(Serial.available()>0)
  {//while there is data available on the serial monitor
    message+=char(Serial.read());//store string from serial command
  }
  if(!Serial.available())
  {
    if(message!="")
    {//if data is available
      Serial.println(message); //show the data
      message=""; //clear the data
    }
  }
  delay(5000); //delay
  */
// Serial.print("AmbientT: ");
// Serial.println(mlx.readAmbientTempC());
// Serial.print("ObjectT: ");
// Serial.println(mlx.readObjectTempC());
// delay(1000);
 /*
  Serial1.write("ts.getCelsius(): ");      Serial1.flush();
      
         dtostrf(ts.getCelsius(),2,2,output);
         Serial1.write(output);      Serial1.flush();
         Serial1.write("feed back temprature");  Serial1.flush();
         Serial1.write("AmbientT: ");      Serial1.flush();
         dtostrf(mlx.readAmbientTempC(),2,2,output);
         Serial1.write(output);      Serial1.flush();
         Serial1.write("ObjectT: ");      Serial1.flush();
          dtostrf(mlx.readObjectTempC(),2,2,output);
         Serial1.write(output);      Serial1.flush();
         Serial1.write((byte)END_BYTE);      Serial1.flush();
         
         delay(2000);
         */
  if (readline(Serial1.read(), buffer, BUFFER_SIZE) > 0) {
    Serial.write("first byte is:");
    Serial.println(buffer[0]);
    
    if(buffer[0]==(byte)'a')
    {      
       Serial1.print("carry out command a");
     
     //carryOutCommands(commands[buffer[1]-(byte)'0']);
      carryOutCommands(commands[buffer[1]]);
      Serial1.write((byte)END_BYTE);
    }
    else if (buffer[0]==(byte)'b')
    {    
      // Serial1.write("carry out command b dfsdf sdfsi 123456 abce efg hij k lmn opq rst uvw xyz hello this is end");
       
      if(buffer[1]==1)
      {      
         reportTemperature(26);  
      }
    }
    
    //  Serial1.print((byte)END_BYTE); android receive as 127 three bytes 49 50 55
   // Serial1.write((byte)END_BYTE);
      
    
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
  if(t>tlimit)
    return true;
  else
    return false;
}

void reportMessage(char * msg)
{
   Serial1.write(msg);       
   Serial1.write((byte)END_BYTE);      //Serial1.flush();
}

bool reportTemperature(double tlimit)
{
  //ds1820
    ds1820.requestTemperatures(); // Send the command to get temperatures  
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial1.write("ds1820: ");
  temperature=ds1820.getTempCByIndex(0);
  dtostrf(temperature,2,2,output);  
  Serial1.write(output); 
  Serial1.write((byte)END_BYTE);      //Serial1.flush();
   if(isTemperatureReached(temperature, tlimit))
   {
     Serial1.write("tpr");   //means temprature reached   //Serial1.flush();
     dtostrf(temperature,2,2,output);  
     Serial1.write(output); 
     Serial1.write((byte)END_BYTE);      //Serial1.flush();
     return true;
   }
   else
   {
     return false;
   }
  //Serial1.write(" ");
    /*
         Serial1.write("ts.getCelsius(): ");      //Serial1.flush();      
         dtostrf(ts.getCelsius(),2,2,output);
         Serial1.write(output);      //Serial1.flush();
         Serial1.write("feed back temprature");  //Serial1.flush();
         Serial1.write("AmbientT: ");      //Serial1.flush();
         dtostrf(mlx.readAmbientTempC(),2,2,output);
         Serial1.write(output);      //Serial1.flush();
         Serial1.write("ObjectT: ");      //Serial1.flush();
          dtostrf(mlx.readObjectTempC(),2,2,output);
         Serial1.write(output);      //Serial1.flush();
         */
         
         /*
   Serial1.write("Max6675: ");      
   dtostrf(ts.getCelsius(),2,2,output);
   Serial1.write(output);      //Serial1.flush();
   Serial1.write(" ");
   Serial1.write((byte)END_BYTE);      //Serial1.flush();
   */
   /*
   if(isTemperatureReached(mlx.readObjectTempC()))
   {
        Serial1.write("temprature reached");      //Serial1.flush();
        Serial1.write((byte)END_BYTE);      //Serial1.flush();
   }
   else
   {
        Serial1.write("AmbientT: ");      //Serial1.flush();
        dtostrf(mlx.readAmbientTempC(),2,2,output);
        Serial1.write(output);      //Serial1.flush();
        Serial1.write("ObjectT: ");      //Serial1.flush();
        dtostrf(mlx.readObjectTempC(),2,2,output);
        Serial1.write(output);      //Serial1.flush();
        Serial1.write((byte)END_BYTE);      //Serial1.flush();
   }
   */
   //return false;
}



 

