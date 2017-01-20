void setup(){
    Serial2.begin(9600);
}
void speech(){
  unsigned char i = 0;
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
}

void loop(){
    speech();
    delay(3000);
}

