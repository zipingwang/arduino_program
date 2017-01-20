#ifndef DISH_H
#define DISH_H
const int BUFFER_SIZE=100;

const char kipGroenten[]={0xCA, 0xB2, 0xBD, 0xF5, 0xBC, 0xA6, 0x00}; //什锦鸡
const char chaohuanggua2[]={0xB3, 0xB4, 0xBB, 0xC6, 0xB9, 0xCF, 0x00}; //炒黄瓜
const char TestCookingPlate[]={0xB2, 0xE2, 0xCA, 0xD4, 0xB5, 0xE7, 0xB4, 0xC5, 0xC2, 0xAF, 0x00}; //测试电磁炉
const char TestCookingPlateOnOff[]={0xB2, 0xE2, 0xCA, 0xD4, 0xB5, 0xE7, 0xB4, 0xC5, 0xC2, 0xAF, 0xBF, 0xAA, 0xB9, 0xD8, 0x00}; //测试电磁炉开关

const int PAGE_DISH_SIZE=8; //number must be same as number of items in the array

const char * page_dish[PAGE_DISH_SIZE]={
"Cooking0",
"Cooking10",
"Cooking15",
"Cooking20",
"Cooking25",
"Cooking30",
"Cooking45",
"Cooking60"
};

unsigned char commands[PAGE_DISH_SIZE][BUFFER_SIZE]={
{'A',1,'A',2,'A',13,'O',1,90,'F',1,0,'A',4,'ÿ'}, //
{'A',1,'A',2,'A',13,'O',1,90,'F',1,10,'A',4,'ÿ'}, //
{'A',1,'A',2,'A',13,'O',1,90,'F',1,15,'A',4,'ÿ'}, //
{'A',1,'A',2,'A',13,'O',1,90,'F',1,20,'A',4,'ÿ'}, //
{'A',1,'A',2,'A',13,'O',1,90,'F',1,25,'A',4,'ÿ'}, //
{'A',1,'A',2,'A',13,'O',1,90,'F',1,30,'A',4,'ÿ'}, //
{'A',1,'A',2,'A',13,'O',1,90,'F',1,40,'A',4,'ÿ'}, //
{'A',1,'A',2,'A',13,'O',1,90,'F',1,60,'A',4,'ÿ'}
};


#endif

