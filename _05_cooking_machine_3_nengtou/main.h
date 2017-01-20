#ifndef MAIN_H
#define MAIN_H

volatile boolean _requestHandled=false;
volatile boolean _interruptedByUser=false;

int _adjustTime=0;
boolean _adjustTimeDone=false;
boolean _requestAdjustTime=false;

const int BUFFER_SIZE=100;
unsigned char _commands[BUFFER_SIZE];

int const STEPS_PER_TURN=512;
int const UNIT_PER_TURN=16; //16 holes in one cylinder turn
//volatile boolean _openCoverReached=false;
#endif
