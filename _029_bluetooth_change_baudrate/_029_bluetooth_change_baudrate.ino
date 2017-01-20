//http://42bots.com/tutorials/hc-06-bluetooth-module-datasheet-and-configuration-with-arduino/
//http://silabs.org.ua/bc4/hc06.pdf
String message; //string that stores the incoming message
String command = ""; // Stores response of the HC-06 Bluetooth device

void setup()
{
   // Open serial communications:
  Serial.begin(9600);
  Serial.println("Type AT commands!");

  Serial1.begin(57600); //set baud rate. this is the bluetooth baud rate, default is 9600, if you have changed to some value else, like 57600, change here.
}


void loop() {
  // Read device output if available.
  if (Serial1.available()) {
    while(Serial1.available()) { // While there is more to be read, keep reading.
      command += (char)Serial1.read();
    }
    
    Serial.println(command);
    command = ""; // No repeats
  }
  
  // Read user input if available.
  if (Serial.available()){
    delay(10); // The delay is necessary to get this working!
    Serial1.write(Serial.read());
  }
}
