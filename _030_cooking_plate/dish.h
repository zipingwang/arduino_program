#ifndef DISH_H
#define DISH_H
const int BUFFER_SIZE=100;

const char kipGroenten[]={0xCA, 0xB2, 0xBD, 0xF5, 0xBC, 0xA6, 0x00}; //什锦鸡
const char chaohuanggua2[]={0xB3, 0xB4, 0xBB, 0xC6, 0xB9, 0xCF, 0x00}; //炒黄瓜
const char TestCookingPlate[]={0xB2, 0xE2, 0xCA, 0xD4, 0xB5, 0xE7, 0xB4, 0xC5, 0xC2, 0xAF, 0x00}; //测试电磁炉
const char TestCookingPlateOnOff[]={0xB2, 0xE2, 0xCA, 0xD4, 0xB5, 0xE7, 0xB4, 0xC5, 0xC2, 0xAF, 0xBF, 0xAA, 0xB9, 0xD8, 0x00}; //测试电磁炉开关

const int PAGE_DISH_SIZE=5; //number must be same as number of items in the array

const char * page_dish[PAGE_DISH_SIZE]={
"back",
kipGroenten,
chaohuanggua2,
TestCookingPlate,
TestCookingPlateOnOff
};

unsigned char commands[PAGE_DISH_SIZE][BUFFER_SIZE]={
{}, //back
{'C',11,'A',6,'C',1,'A',7,'A',8,'C',2,'B',103, '(', 'B',100,50,'B',101,60,'B',102,0, ')','A',11,'Q',1,'N','B',1,5,'Q',2,'F',6,150,'A',9,'A',10,'C',4,'ÿ'}, //kipGroenten
{'A',1,'A',2,'A',3,2,'O',75,'C',7,'F',10,'C',7,'F',10,'C',1,'C',6,'B',100,250,'B',100,250,'B',100,250,'B',100,250,'B',100,250,'F',10,'C',5,'C',3,'N','B',1,15,'C',6,'F',20,'C',3,'F',10,'A',5,3,'F',10,'A',5,2,'F',20,'C',4,'A',4,'ÿ'}, //chaohuanggua2
{'A',1,'A',2,'A',13,'O',2,90,180,'F',1,20,'A',4,'ÿ'}, //TestCookingPlate
{'A',1,'A',4,'ÿ'}, //TestCookingPlateOnOf
};


#endif

