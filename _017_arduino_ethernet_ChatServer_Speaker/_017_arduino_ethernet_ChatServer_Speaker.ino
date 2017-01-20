/*
 Chat  Server
 
 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x74, 0x84 };
IPAddress ip(192,168,1, 177);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);


// telnet defaults to port 23
EthernetServer server(23);
boolean alreadyConnected = false; // whether or not the client was connected previously

String inputString="";

int pinPlayE=7; //press one, play whole sound
int pinPlayL=8; //play sound if keep pressed

void setup() { 
  pinMode(pinPlayE, OUTPUT);
  digitalWrite(pinPlayE, LOW);
  
  inputString.reserve(200);
  // initialize the ethernet device
  //Ethernet.begin(mac, ip, gateway, subnet);
  Ethernet.begin(mac, ip); //no need gateway and subnet, if pc and arduino connet to a switch.
  // start listening for clients
  server.begin();
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clead out the input buffer:
      client.flush();    
      Serial.println("We have a new client");
      client.println("Hello, client!"); 
      alreadyConnected = true;
    } 

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
         if (thisChar == '\n')
         {
           digitalWrite(pinPlayL, LOW);
           
           digitalWrite(pinPlayE, LOW); //close previous one if exists
           digitalWrite(pinPlayE, HIGH);
           delay(1000);
           digitalWrite(pinPlayE, LOW);
         }
         else if (thisChar == 'b')
         {
           digitalWrite(pinPlayE, LOW);
           digitalWrite(pinPlayL, LOW); //close previous one if exists
           delay(100);
           digitalWrite(pinPlayL, HIGH);
         }
         else if(thisChar == 'e')
         {
           digitalWrite(pinPlayE, LOW);
            digitalWrite(pinPlayL, LOW);
         }
         /*
         if (thisChar == '\n')
         {
           //for(int i=0; i<inputString.length(); i++)
           //{
           //  server.write(inputString[i]);
           //}
           digitalWrite(pinPlayE, HIGH);
           delay(1000);
           digitalWrite(pinPlayE, LOW);
           inputString="";
         }
         else
         {
           inputString += thisChar;
         }
         */
      // echo the bytes back to the client:
      //server.write(thisChar);
      
      // echo the bytes to the server as well:
      Serial.write(thisChar);
    }
  }
}



