
#include "dish.h"
const int BUFFER_SIZE=100;

const int PAGE_MAIN=0;
const int PAGE_DISH=1;
const int PAGE_CONFIG=3;
const int PAGE_COOKING_DONE=4;
const int PAGE_CONFIG_DONE=5;
const int PAGE_MEASURE_TEMPRATURE=6;


const int PAGE_MAIN_SIZE=2; //number must be same as number of items in the array
const char * page_main[PAGE_MAIN_SIZE]={
  "cooking", 
  "config"
};

const int PAGE_DISH_SIZE=8; //number must be same as number of items in the array
const char * page_dish[PAGE_DISH_SIZE]={
  "back",
  "cooking egg", 
  "spagetti", 
  "wok",
  "kipCurry",
  "kipGroenten",
  "koeloekai",
  "chao huang gua"
};

//255 is end of command byte(char)
unsigned char commands[PAGE_DISH_SIZE][BUFFER_SIZE]={
  {}, //back
  {'A',1,'B',1,5, 'B',100,200,'F',5, 'B',103, '(', 'B',100,200,'B',101,200,'B',102,200, ')','A',4,'ÿ'}, //"cooking egg", 
  {'A',1,'B',100,200,'A',4,'ÿ'}, //"spagetti", 
  {'A',1,'B',103, '(', 'B',100,200,'B',101,200,'B',102,200, ')','A',4,'ÿ'},//"wok",
  {'A', 1, 0x66, 11, 'A', 4, 255},//"kipCurry",
  {'A', 1, 'C',2, 'A',2, 'A',3,2, 'O', 70, 'B',1, 'C',1, 'C',2, 'B',100, 'N', 'F',30, 'C',3, 'F',120, 'A',4, 'C',4, 255},//"kipGroenten",
  {'A', 1, 0x66, 11, 'A', 4, 255},//"koeloekai"
  {'A',1,'A',2,'A',3,2,'O',75,'C',7,'F',10,'C',8,'F',10,'C',1,'C',6,'B',100,250,'B',100,250,'B',100,250,'B',100,250,'F',10,'C',5,'C',3,'N','B',1,15,'C',6,'F',20,'C',3,'F',10,'A',5,3,'F',10,'A',5,2,'F',20,'C',4,'A',4,'ÿ'} //"chao huang gua"
};

const int PAGE_CONFIG_SIZE=15;
const char * page_config[PAGE_CONFIG_SIZE]={
  "back",
  "pomp oil", 
  "move total",
  "turn salt",
  "move oil",
  "measure T",
  "plate on/off",
  "plate mode",
  "plate T + ",
  "plate T -",
  "turn salt(empty)",
  "turn sugar(empty)",
  "turn weijing(empty)",
  "turn sugar",
  "turn weijing"
};
