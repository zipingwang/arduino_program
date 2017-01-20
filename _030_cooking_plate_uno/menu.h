
#include "dish.h"

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

const int PAGE_CONFIG_SIZE=25; //0 based, max 14 char per line(16-2)
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
  "turn salt(empt", //(empty)
  "turn sugar(emp",
  "turn weijing(e",
  "turn sugar",
  "turn weijing",
  "cover open",
  "cover close", 
  "heat bottom on",
  "heat top on",
  "stir motor 10s",
  "heat bottom of",
  "heat top off",
  "Preheating",
  "openCloseRepea",
  "selfTest"
};
