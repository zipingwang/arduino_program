//
//must change standaart adafruit file: Adafruit_CC3000.cpp
//I put comments //ziping in the file where changed
// start simpleTcp.exr, port 9100
//check firewall
//note ip address where simple.tcp runs. change ip bellow
//cc3000 client, can use DHCP or fixed ip. if use fixed ip. check not conflict with server ip

/*************************************************** 
  This is an example for the Adafruit CC3000 Wifi Breakout & Shield

  Designed specifically to work with the Adafruit WiFi products:
  ----> https://www.adafruit.com/products/1469

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 
 /*
This example does a test of the TCP client capability:
  * Initialization
  * Optional: SSID scan
  * AP connection
  * DHCP printout
  * DNS lookup
  * Optional: Ping
  * Connect to website and print out webpage contents
  * Disconnect
SmartConfig is still beta and kind of works but is not fully vetted!
It might not work on all networks!
*/
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
//#include "utility/debug.h"
#include <SD.h>

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000= Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "WIFI_ZW"           // cannot be longer than 32 characters!
#define WLAN_PASS       "shanghai"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.

// What page to grab!
//#define WEBSITE     "www.adafruit.com" //"54.175.219.8" //"httpbin.org"  //"207.58.139.247"  //"www.adafruit.com"
//#define WEBPAGE      "/testwifi/index.html" //"/cache/60"


/**************************************************************************/
/*!
    @brief  Sets up the HW and the CC3000 module (called automatically
            on startup)
*/
/**************************************************************************/

uint32_t serverIp= ((uint32_t)192<<24) | ((uint32_t)168<<16) | ((uint32_t)1<<8) | 3; // 3232235784; //192.168.1.8;
//ip=192;
//ip=ip<<24

void setup(void)
{
  //cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER);
  Serial.begin(115200);
  //Serial.println(F("Hello, CC3000!\n")); 

  //Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
  
  /* Initialise the module */
  cc3000.begin();
  //Serial.println(F("\nInitializing..."));
  //if (!cc3000.begin())
  //{
  //  Serial.println(F("Couldn't begin()! Check your wiring?"));
  //  while(1);
  //}
  
   //ziping
   /* Delete any old connection data on the module */
  //Serial.println(F("\nDeleting old connection profiles"));
  //if (!cc3000.deleteProfiles()) {
    //Serial.println(F("Failed!"));
    //while(1);
  //}
  //end ziping
  
  // Optional SSID scan
  // listSSIDResults();
  
  //ziping
  cc3000.setDHCP();
  //                          192.168.1.3, 255.255.255.0, 192.168.1.1,192.168.1.1
  //conver ip string to int http://www.aboutmyip.com/AboutMyXApp/IP2Integer.jsp?ipAddress=192.168.1.1
  //cc3000.setStaticIPAddress(3232235779, 4294967040, 3232235777, 3232235777); 
  //end ziping
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  
  //Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
  //if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
  //  Serial.println(F("Failed!"));
  //  while(1);
  //}
   
  //Serial.println(F("Connected!"));
  
  
  /* Wait for DHCP to complete */
  //Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */  
  //while (! displayConnectionDetails()) {
  //  delay(1000);
  //}

  //ip = 0;
  //ip=3232235784; //192.168.1.8
  //ip=917494536; //(54L << 24) | (175L<<16) | (219<<8) | (8); //54.175.219.8 //917494536
  // Try looking up the website's IP address
  /*
  Serial.print(WEBSITE); Serial.print(F(" -> "));
  while (ip == 0) {
    if (! cc3000.getHostByName(WEBSITE, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }
  */
  //Serial.println("\nServerIp:");
  //cc3000.printIPdotsRev(serverIp);
  
  // Optional: Do a ping test on the website
  /*
  Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");  
  replies = cc3000.ping(ip, 5);
  Serial.print(replies); Serial.println(F(" replies"));
  */  

  /* Try connecting to the website.
     Note: HTTP/1.1 protocol is used to keep the server from closing the connection before all data is read.
  */
  sendToServer("I an arduino cc3000");
  Adafruit_CC3000_Client www = cc3000.connectTCP(serverIp, 9100);
  
  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time your try to connect ... */
  //Serial.println(F("\n\nDisconnecting")); 
  //cc3000.disconnect();
  
}

void loop(void)
{
  //delay(1000);
}



/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
/*
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv);
  Serial.print(F("\nmy IP: ")); cc3000.printIPdotsRev(ipAddress);
  //if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  //{
    //Serial.println(F("Unable to retrieve the IP Address!\r\n"));
  //  return false;
  //}
  //else
  //{
  //  Serial.print(F("\nmy IP: ")); cc3000.printIPdotsRev(ipAddress);
    //Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    //Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    //Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    //Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    //Serial.println();
  //  return true;
  //}
}
*/

void sendToServer(char* msg)
{
    Adafruit_CC3000_Client www = cc3000.connectTCP(serverIp, 9100);
    if (www.connected()) {
       www.fastrprint(msg);    
    } 
    //else {
      //Serial.println(F("Connection failed"));    
    //  return;
    //}
    www.close();
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    //inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == 'a') {
      Adafruit_CC3000_Client www = cc3000.connectTCP(serverIp, 9100);
    if (www.connected()) {
       www.fastrprint(F("command a")); 
       if(www.read()==97)
      {
        Serial.println("re");
      } 
       www.close();  
    } else {
      //Serial.println(F("Connection failed"));    
      return;
    }
      
    //Serial.println(F("-------------------------------------"));
  
 
  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time your try to connect ... */
      //Serial.println(F("\n\nsocket Disconnecting"));
       
      //cc3000.disconnect();
    }
    else if(inChar=='b')
    {
         TestSD();
    }
    
  }
}


void TestSD()
{
  
  File myFile;
  // Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   SD.begin(4);
  //if (!SD.begin(4)) {
    //Serial.println("initialization failed!");
  //  return;
  //}
  Serial.println("initialization done.");
   /*
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    //Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
	// close the file:
    myFile.close();
    //Serial.println("done.");
  } 
  //else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  //}
  */
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
      Adafruit_CC3000_Client www = cc3000.connectTCP(serverIp, 9100);      
    //Serial.println("test.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
       int c=myFile.read();
    	Serial.write(c);
    
         if (www.connected()) {
           String s = String((char)c);
            char const *pchar = s.c_str();  //use char const* as target type
             www.fastrprint(pchar);    
          } else {
              Serial.println(F("Connection failed")); 
          }   
         
    }
    if (www.connected()){
    www.close();
    }
    // close the file:
    myFile.close();
  } 
  //else {
  	// if the file didn't open, print an error:
  //  Serial.println("error opening test.txt");
  //}
  
}

