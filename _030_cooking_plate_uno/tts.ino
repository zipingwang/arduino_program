//serial2 in arduino mega is pins 17 (RX) and 16 (TX)
//connect rxd to pin 16, txd to pin rx. txd can be open, not needed

void playDoneSound()
{
  unsigned char i = 0;
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
     // Serial2.write(head[i]);
  }
}
