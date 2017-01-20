//this sketch use arduino mega. Serial1 use pin18, pin19

//http://www.instructables.com/id/Add-bluetooth-to-your-Arduino-project-ArduinoHC-06/?ALLSTEPS

//http://42bots.com/tutorials/how-to-connect-arduino-uno-to-android-phone-via-bluetooth/

byte END_BYTE=127; //java max byte is 127, from android send byte max value is 127. if received this byte, it means end of message
const int BUFFER_SIZE=100;
byte buffer[BUFFER_SIZE];
int count=0;

//////////////////////////////////////////////////////////////////////////////

void setup() {   
  Serial.begin(9600);
  
  //The Arduino Mega has three additional serial ports: Serial1 on pins 19 (RX) and 18 (TX), Serial2 on pins 17 (RX) and 16 (TX), Serial3 on pins 15 (RX) and 14 (TX). 
  Serial1.begin(57600); //set baud rate default is 9600 for HC-06, see _029_bluetooth_change_baudrate how to change baud rate
}


 void loop()
{ 
   delay(2000);
    Serial1.write("count:");
    Serial1.print(count);
    Serial1.write(" ts.getCelsius(): ");    
    Serial1.write("feed back temprature");  
    Serial1.write("AmbientT: ");      
         //dtostrf(mlx.readAmbientTempC(),2,2,output);
         //Serial1.write(output);    
    Serial1.write("ObjectT: ");     
         // dtostrf(mlx.readObjectTempC(),2,2,output);
    //Serial1.write(output);      
    Serial1.write((byte)END_BYTE);   
     Serial1.flush();
     
    delay(2000);
    
    count++;
         
  if (readline(Serial1.read(), buffer, BUFFER_SIZE) > 0) {
    //reveice something from android
    Serial.write("first byte is:");
    Serial.println(buffer[0]);
    
    if(buffer[0]==(byte)'a')
    {     
       //send something to android 
       Serial1.write("carry out command a");
       Serial1.write((byte)END_BYTE);
    }
    else if (buffer[0]==(byte)'b')
    {    
      // Serial1.write("carry out command b dfsdf sdfsi 123456 abce efg hij k lmn opq rst uvw xyz hello this is end");
       
      if(buffer[1]==1)
      {
       //send something to android 
       Serial1.write("carry out command b"); 
       Serial1.write("count:");
       Serial1.print(count);
       Serial1.write((byte)END_BYTE); Serial1.flush();
      
      }
    }    
  }
}
    
static int pos = 0;
int readline(int readch, byte *buffer, int len)
{   
   int rpos;
   if (readch >= 0) {
     Serial.write(readch);
     if (pos < len-1) {
         buffer[pos] = readch;  
         pos++;  
         if(readch==END_BYTE)
         {
           rpos = pos;
           Serial.write("end");
           pos=0;
           return rpos;
         }
     }
   }
  // No end of line has been found, so return -1.
  return -1;
}


 

