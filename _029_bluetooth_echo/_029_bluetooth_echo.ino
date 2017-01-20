

//http://www.instructables.com/id/Add-bluetooth-to-your-Arduino-project-ArduinoHC-06/?ALLSTEPS

//http://42bots.com/tutorials/how-to-connect-arduino-uno-to-android-phone-via-bluetooth/
String message; //string that stores the incoming message

void setup()
{
  Serial1.begin(57600); //set baud rate
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
  static char buffer[80];
  if (readline(Serial1.read(), buffer, 80) > 0) {
    Serial1.print("You entered: >");
    Serial1.print(buffer);
    Serial1.println("<");
  }
}
    
    
int readline(int readch, char *buffer, int len)
{
  static int pos = 0;
  int rpos;

  if (readch > 0) {
    switch (readch) {
      case '\n': // Ignore new-lines
        break;
      case '\r': // Return on CR
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (pos < len-1) {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}

