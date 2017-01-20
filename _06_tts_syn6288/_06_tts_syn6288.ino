//serial2 in arduino mega is pins 17 (RX) and 16 (TX)
//connect rxd to pin 16, txd to pin rx. txd can be open, not needed
void setup(){
    Serial2.begin(9600);
}
void speech(){
  unsigned char i = 0;
/*
  //[V9]这是小音量
  unsigned char head[20];
  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x11;
  head[3] = 0x01;
  head[4] = 0x01;
  head[5] = 0x5B;
  head[6] = 0x56;
  head[7] = 0x39;
  head[8] = 0x5D;
  head[9] = 0xD5;
  head[10] = 0xE2;
  head[11] = 0xCA;
  head[12] = 0xC7;
  head[13] = 0xD0;
  head[14] = 0xA1;
  head[15] = 0xD2;
  head[16] = 0xF4;
  head[17] = 0xC1;
  head[18] = 0xBF;
  head[19] = 0x96;
  
  for(i=0; i<20; i++){
    Serial2.write(head[i]);
  }
*/

/*
//sounda
unsigned char head[12];

head[0] = 0xFD;
head[1] = 0x00;
head[2] = 0x09;
head[3] = 0x01;
head[4] = 0x00;
head[5] = 0x73;
head[6] = 0x6F;
head[7] = 0x75;
head[8] = 0x6E;
head[9] = 0x64;
head[10] = 0x61;
head[11] = 0xF7;

for(i=0; i<12; i++){
    Serial2.write(head[i]);
}
 
 */
 /*
//soundl 
unsigned char head[12];

head[0] = 0xFD;
head[1] = 0x00;
head[2] = 0x09;
head[3] = 0x01;
head[4] = 0x00;
head[5] = 0x73;
head[6] = 0x6F;
head[7] = 0x75;
head[8] = 0x6E;
head[9] = 0x64;
head[10] = 0x6C;
head[11] = 0xFA;

for(i=0; i<12; i++){
    Serial2.write(head[i]);
}
*/

//msge
unsigned char head[10];

head[0] = 0xFD;
head[1] = 0x00;
head[2] = 0x07;
head[3] = 0x01;
head[4] = 0x00;
head[5] = 0x6D;
head[6] = 0x73;
head[7] = 0x67;
head[8] = 0x65;
head[9] = 0xE7;

for(i=0; i<10; i++){
    Serial2.write(head[i]);
}


/*
//soundn
unsigned char head[12];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x09;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0x73;
  head[6] = 0x6F;
  head[7] = 0x75;
  head[8] = 0x6E;
  head[9] = 0x64;
  head[10] = 0x6E;
  head[11] = 0xF8;

  for(i=0; i<12; i++){
    Serial.write(head[i]);
  }
  */
}

void loop(){
    speech();
    delay(3000);
}

