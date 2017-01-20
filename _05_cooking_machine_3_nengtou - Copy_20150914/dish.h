#ifndef DISH_H
#define DISH_H

//the command for prepare dishs is in menu.h

//add 0x00 to the end, if want to use void displayLineInLcd(int lineNr, unsigned char * text), because strlen((const char*)text) search null terminated
const char show0[]={0xBC,0xAB,0xBF,0xCD,0xB9,0xA4,0xB7,0xBB, 0x00};//极客工坊
const char kipCurry[]={
  0xBF, 0xA7,
  0xE0, 0xAC,
  0xBC, 0xA6, 0x00
     };                    //咖喱鸡
 const char kipGroenten[]={
  0xCA, 0xB2,
  0xBD, 0xF5,
  0xBC, 0xA6, 0x00
     };                    //什锦鸡
     
#endif
