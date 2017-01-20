void carryOutCommands(unsigned char commands[])
{
  unsigned char nextByte;
  //unsigned char nextChar;
  short value;
  double objectT=0;
  char output[16]; //buffer for temperator

  int * pCount;
  int iCount=0;
  
  int degree;
  long maxTime;
  long timeElapsed;
  
  pCount=&iCount;
  
  doSetIdleSignOff();
  doSetBusySignOn();

  for(iCount=0; iCount<BUFFER_SIZE; iCount++)
  {		
    switch(commands[*pCount])
    {
    case 'A': //cooking plate
      nextByte=GetNextCommand(commands, pCount);
      if(nextByte==1)
      {
        //moveTemperatorSensorOut(); //temporary set here. donn't know where is is the right place.
        //1.turn on cooking plat
        doTurnOnCookingPlate();	
      }
      else if(nextByte==2)
      {
        //2.switch to cooking plate mode
        doSwitchCookingPlateToMode();
      }
      else if(nextByte==3)
      {
        //3.set plate power plus  x times. x can be 1, 2, 3, 4
        value=GetNextValue(commands, pCount);
        //value=1;
        doSetPlatePowerPlus(value);
      }	
      else if(nextByte==4)
      {
        //4.turn off cooking plat
        doTurnOffCookingPlate();
      }
      else if(nextByte==5)
      {
        //5.set plate power min  x times. x can be 1, 2, 3, 4
        value=GetNextValue(commands, pCount);
        //value=1;
        doSetPlatePowerMin(value);
      }	
     
      else if(nextByte==12)
      {
        // 12. nentou motor stir off        
        //doStirMotorOff();
      }		
       else if(nextByte==13)
      {
        //  13. set plate power to maximum     
        //doStirMotorOff();
        doSwitchCookingPlateToMaximum();
      }		
        else if(nextByte==14)
      {
        // set plate power to minimum     
        //doStirMotorOff();
         doSwitchCookingPlateToMinimum();
      }			
      break;   
    case 'F':
      //F.cooking x sconds
      //displayBusyLcd();
      //moveTemperatorSensorOut();
      
      nextByte=GetNextCommand(commands, pCount);
      value=GetNextValue(commands, pCount);
      
      //if(nextByte==2)
      //  doStirMotorOn();
        
      if(nextByte==1 || nextByte==2)
      {
        //1. cooking x sconds
        //2. cooking x sconds with continue stiring 
        for(int icount=0; icount<value; icount++)
        {  
         // dtostrf(mlx.readAmbientTempC(),2,2,output); 
         // displayLineInLcd(1,  output);
        
         // dtostrf(mlx.readObjectTempC(),2,2,output);  
         // displayLineInLcd(2,  output);    
          delay(1000);    //delay(value*1000);  
        }				
      }      
      break;
    case 'O':     
      value=GetNextValue(commands, pCount);
      
      degree=GetNextValue(commands, pCount);;
     
      timeElapsed=0;
      
      if(value==1)
      {
          // 1. heet plate untill x degree   
         maxTime=60000; //1000 minutes          
      }
      else if(value==2)
      {
        // 2. 1. heet plate untill x degree with maximum Y seconds
        maxTime=GetNextValue(commands, pCount);;
      }
      
     
      //displayBusyLcd();  
      //moveTemperatorSensorOut();    
      //objectT=mlx.readObjectTempC();
      reportTemperature(degree);
      //inox 50; iron 85
      //_requestHandled=false; // for test purpose
      //while(objectT<value && !_requestHandled && timeElapsed/1000 < maxTime) //while(objectT<value)
      while(objectT<degree && timeElapsed/1000 < maxTime) //while(objectT<value)
      {
        //Serial.print("*C\tObject = "); Serial.print(objectT); Serial.println("*C");
        delay(2000);
        timeElapsed+=2000;
          
        //objectT=mlx.readObjectTempC();
        if(reportTemperature(degree, timeElapsed/1000))
           break;
        //dtostrf(mlx.readAmbientTempC(),2,2,output); 
        //displayLineInLcd(1,  output);      
        
        //dtostrf(mlx.readObjectTempC(),2,2,output);  
        //displayLineInLcd(2,  output);  
      }
        reportMessage("out O2");
      //moveTemperatorSensorOut();
        
      //displayBusyLcd();
      break;
      
   case 'P': //status light
      nextByte=GetNextCommand(commands, pCount);
      if(nextByte==1)
      {
       doSetBusySignOn();
      }
      else if(nextByte==2)
      {
        doSetBusySignOff();
      }
       else if(nextByte==3)
      {
        doSetIdleSignOn();
      }
       else if(nextByte==4)
      {
        doSetIdleSignOff();
      }
      break;
    case 255: //255 means end of command
      //end of command
      iCount=BUFFER_SIZE;
      break;
    }
  }

  doSetBusySignOff();
  doSetIdleSignOn();
}



unsigned char GetNextCommand(unsigned char commands[], int * currentPos)
{
  (*currentPos)++;
  return commands[*currentPos];	
}

short GetNextValue(unsigned char commands[], int * currentPos)
{
  /*
	short value;
   	(*currentPos)++;
   	value= commands[*currentPos] *254;	//255 is end of command char
   	(*currentPos)++;
   	value=value+commands[*currentPos];
   	return value;
   */
  (*currentPos)++;
  return commands[*currentPos];        
}


void doTurnOnCookingPlate()
{
  touchCookingPlateWire(pinCookingPlateOnOff);
}

void doTurnOffCookingPlate()
{
  touchCookingPlateWire(pinCookingPlateOnOff);
}

void doSetPlatePowerPlus(int value)
{
  for(int icount=0; icount<value; icount++)
  {
    touchCookingPlateWire(pinCookingPlatePowerPlus);
  }
}

void doSetPlatePowerMin(int value)
{
  for(int icount=0; icount<value; icount++)
  {
    touchCookingPlateWire(pinCookingPlatePowerMin);
  }
}

void doSwitchCookingPlateToMode()
{
  touchCookingPlateWire(pinCookingPlateMode);   
}

void doSwitchCookingPlateToMaximum()
{
  touchCookingPlateWire(pinCookingPlateMaximum);   
}

void doSwitchCookingPlateToMinimum()
{
  touchCookingPlateWire(pinCookingPlateMinimum);   
}

void doSetBusySignOn()
{
  //connectRelayPin(pinBusyLed);  
  //digitalWrite(pinBusyLed, HIGH);
}

void doSetBusySignOff()
{
  //disconnectRelayPin(pinBusyLed); 
  //digitalWrite(pinBusyLed, LOW);
}

void doSetIdleSignOn()
{
  //connectRelayPin(pinIdleLed);  
  //digitalWrite(pinIdleLed, HIGH);
}

void doSetIdleSignOff()
{
  //connectRelayPin(pinIdleLed); 
  //digitalWrite(pinIdleLed, LOW); 
}



