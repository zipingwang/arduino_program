void carryOutCommands(unsigned char commands[])
{
	unsigned char nextByte;
	//unsigned char nextChar;
	short value;
    double objectT=0;

	int * pCount;
	int iCount=0;
	pCount=&iCount;

    doSetBusySignOn();
        
	for(iCount=0; iCount<BUFFER_SIZE; iCount++)
	{		
		switch(commands[*pCount])
		{
		case 'A': //cooking plate
                        nextByte=GetNextCommand(commands, pCount);
                        if(nextByte==1)
                        {
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
                          doSetPlatePowerPlus(value);
                        }	
                        else if(nextByte==4)
                        {
                          //1.turn off cooking plat
  			  doTurnOffCookingPlate();
                        }
                        else if(nextByte==5)
                        {
                          //3.set plate power min  x times. x can be 1, 2, 3, 4
			  value=GetNextValue(commands, pCount);
                          doSetPlatePowerMin(value);
                        }		
			break;
		case 'B':			    
				nextByte=GetNextCommand(commands, pCount);
				value=GetNextValue(commands, pCount);
				if(nextByte==1)
				{
					//B1 put oil to plate
					doPutOilToPlate(value);					
				} 
				else if(nextByte==2)
				{
					//2 put laochou to plate
					doPutLaoChouToPlate(value);	
					//continue for value second
					//stop put laochou to plate
				}
				else if(nextByte==3)
				{
					//3 put shenchou to plate
					doPutShenChouToPlate(value);
					//continue for value second
					//stop put shenchou to plate
				}
				else if(nextByte==4)
				{
					//4 put water to plate
					doPutWaterToPlate(value);
					//continue for value second
					//stop put shenchou to plate
				}
				else if(nextByte==5)
				{
					//5   put jiu to plate
					doPutJiuToPlate(value);
					//continue for value second
					//stop put shenchou to plate
				}
				else if(nextByte==6)
				{
					// 6 put mayou to plate
					doPutMaYouToPlate(value);
					//continue for value second
					//stop put shenchou to plate
				}
				else if(nextByte==7)
				{
					//  7 put qianfeng to plate
					doPutQianFengToPlate(value);
					//continue for value second
					//stop put shenchou to plate
				}
				else if(nextByte==100)
				{
					//100 take salt 2g
					doTakeSalt(value);
					//continue for value second
					//stop put shenchou to plate
				}
				else if(nextByte==101)
				{
					//101 take sugar 10g
					doTakeSugar(value);
					//continue for value second
					//stop put shenchou to plate
				}
				else if(nextByte==102)
				{
					// 102 take weijing 2g 
					doTakeWeiJing(value);
					//continue for value second
					//stop put shenchou to plate
				}
				break;
		case 'C':
                        //dispay text in lcd
                        nextByte=GetNextCommand(commands, pCount);
                        if(nextByte==1)
                        {
                          //1. put food into plate
                          displayPutFootInCookingPlateLcd();                          
                        }   
                        else if(nextByte==2)
                        {
                          //2. busy message
                          displayBusyLcd();                          
                        }     
                        else if(nextByte==3)
                        {
                          //3. ask for stiring
                          displayAskStiringPlateLcd();                          
                        } 
                        else if(nextByte==4)
                        {
						   //4. done message
							displayCookingDone();  
                        }                          			
			break;
		case 'D':
			//request use to handel message
			//waiting for press ok button by user
                        _requestHandled=false;				
                        while(!_requestHandled)
			{
			    delay(100);
                            doSetBusySignOff();
                            delay(100);
                            doSetBusySignOn();
			}
				doSetBusySignOn();
				break;
			case 'E':
				//
				//
				break;
			case 'F':
				//F.cooking x sconds
				value=GetNextValue(commands, pCount);
				delay(value*1000);
				break;
			case 'I':
				//I.put ingredients to pan
				break;
			case 'J':
				
				break;
			case 'K':
				 //			
				break;
			case 'M':
				//
				break;
                        case 'N':
				//N move total(ingredients)
                                moveTotal();
				break;
                        case 'O':
    			        //O heet plate untill x degree
                               value=GetNextValue(commands, pCount);
                               objectT=mlx.readObjectTempC();
                               //inox 50; iron 85
                               while(objectT<value)
                               {
                                 //Serial.print("*C\tObject = "); Serial.print(objectT); Serial.println("*C");
                                 delay(1000);
                                 objectT=mlx.readObjectTempC();
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

//value is in seconds
void doPutOilToPlate(int value)
{
  connectRelayPin(pinOil, value*1000);  
}

//value is in seconds
void doPutLaoChouToPlate(int value)
{
  connectRelayPin(pinLaoChou, value*1000);  
}

//value is in seconds
void doPutShenChouToPlate(int value)
{
  connectRelayPin(pinShenChou, value*1000);  
}

//value is in seconds
void doPutWaterToPlate(int value)
{
  connectRelayPin(pinWater, value*1000);  
}

//value is in seconds
void doPutJiuToPlate(int value)
{
  connectRelayPin(pinJiu, value*1000);  
}

//value is in seconds
void doPutMaYouToPlate(int value)
{
  connectRelayPin(pinMaYou, value*1000);  
}

//value is in seconds
void doPutQianFengToPlate(int value)
{
  connectRelayPin(pinQianFeng, value*1000);  
}

void doTakeSalt(int turns){   
    
   //positive clockwise   
   //660 is one turn(measured by practice)
   // shiftMotor.goSteps(4096, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
    shiftMotor.goSteps(turns, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
    shiftMotor.sleepMotor();
    delay(1000); //maybe can be removed

}

void moveTotal()
{
    //move
    shiftMotor.goSteps(-220, false, 4, 5, 6, 7, 2, motorSpeed28, noStopPin); //move
    shiftMotor.sleepMotor();
    delay(1000);
    
    //turn
    shiftMotor.goSteps(-300, false, 0, 1, 2, 3, 2, motorSpeed28, noStopPin); //final turn
    shiftMotor.sleepMotor();
    delay(1000);
    
     //turn back
    shiftMotor.goSteps(500, false, 0, 1, 2, 3, 2, motorSpeed28, turnPinTotal); //final turn
    shiftMotor.sleepMotor();
    delay(1000);
    
    //move back
    shiftMotor.goSteps(500, false, 4, 5, 6, 7, 2, motorSpeed28, stopPinTotal); //move
    shiftMotor.sleepMotor();
    delay(1000);
}

void doTakeSugar(int turns){ 
    shiftMotor.goSteps(turns, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
    shiftMotor.sleepMotor();   
    delay(1000);
}

void doTakeWeiJing(int turns){ 
    shiftMotor.goSteps(turns, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
    shiftMotor.sleepMotor();   
    delay(1000);
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

void displayPutFootInCookingPlateLcd()
{
   LCDA.CLEAR();
   displayLineInLcd(0, "put food in plate");    
   currentIndex=0;   
}

void displayAskStiringPlateLcd()
{
   LCDA.CLEAR();
   displayLineInLcd(0, "stiring please");    
   currentIndex=0;    
}

void displayCookingDone()
{                  
   LCDA.CLEAR();//清屏
   delay(200);
   //LCDA.DisplayString(0,0,cookingDone,AR_SIZE(cookingDone));
   currentPage=PAGE_COOKING_DONE;
   displayLcd(PAGE_COOKING_DONE, 0);    
}

void doSetBusySignOn()
{
  connectRelayPin(pinBusyLed);  
}

void doSetBusySignOff()
{
  disconnectRelayPin(pinBusyLed); 
}

void doSetIdleSignOn()
{
   connectRelayPin(pinIdleLed);  
}

/*
void doSetRequestSign()
{
  connectRelayPin(pinBusyLed);  
}

void doSetRequestSignOff()
{
  connectRelayPin(pinBusyLed);  
}
*/
