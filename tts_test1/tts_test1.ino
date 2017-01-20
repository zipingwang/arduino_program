//#include <Syn6288.h>
#define uchar unsigned char

//Syn6288 myTalk;



void setup() {
Serial.begin(9600); // set up Serial library at 9600 bps
Serial.println("Motor test!");
Serial2.begin(9600);
}

void loop() {
 // myTalk.Speech("文字1",1,2);
  //Serial.println("myTalk1");
//delay(3000);
//  myTalk.Speech("文字2",1);
   //Serial.println("myTalk2");
//delay(3000);
//  myTalk.Speech("文字3");
//delay(3000);
DoSpeech("ab");
delay(3000);
}

void DoSpeech(const char * txt)
{
 uchar bkm=0;
 uchar nPort=2;
 uchar _nBkm;
 uchar _nPort;
uchar i=0;
uchar head[5];
uchar x=0;

head[0]=0xfd;
head[1]=0x00;
head[3]=0x01;

_nPort=nPort;
_nBkm=bkm % (5 + 1);
head[2]=sizeof(txt) + 3;
head[4]=_nBkm << 3;
Serial2.write(head,5);
for(i=0;i<5;i++)
{
x=x^head[i];
if(_nPort==1)
Serial2.write(head[i]);

else if(_nPort==2)
Serial2.write(head[i]);
}

for(i=0;i<head[2];i++)
{
x=x^txt[i];
if(_nPort==1)
Serial2.write(txt[i]);
else if(_nPort == 2)
Serial2.write(txt[i]);
}
if(_nPort == 1)
Serial2.write(x);
else if(_nPort == 2)
Serial2.write(x);
}


