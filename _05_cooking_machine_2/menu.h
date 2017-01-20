
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

const int PAGE_DISH_SIZE=7; //number must be same as number of items in the array
const char * page_dish[PAGE_DISH_SIZE]={
  "back",
  "cooking egg", 
  "spagetti", 
  "wok",
  "kipCurry",
  "kipGroenten",
  "koeloekai"
};

//255 is end of command byte(char)
unsigned char commands[PAGE_DISH_SIZE][BUFFER_SIZE]={
  {}, //back
  {'A', 1, 'C',2, 'F',5, 'C',1, 'D', 'C',2, 'F',3, 'A',4, 255}, //"cooking egg", 
  {'A', 1, 0x66, 11, 'A', 4, 255}, //"spagetti", 
  {'A', 1, 0x66, 11, 'A', 4, 255},//"wok",
  {'A', 1, 0x66, 11, 'A', 4, 255},//"kipCurry",
  {'A', 1, 'C',2, 'A',2, 'A',3,2, 'O', 70, 'B',1, 'C',1, 'C',2, 'B',100, 'N', 'F',30, 'C',3, 'F',120, 'A',4, 'C',4, 255},//"kipGroenten",
  {'A', 1, 0x66, 11, 'A', 4, 255}//"koeloekai"
};

const int PAGE_CONFIG_SIZE=6;
const char * page_config[PAGE_CONFIG_SIZE]={
  "back",
  "pomp oil", 
  "move total",
  "turn salt",
  "move oil",
  "measure T"
};
