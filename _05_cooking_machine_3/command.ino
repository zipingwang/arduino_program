void carryOutCommands(unsigned char commands[])
//void CommandProcessor::carryOutCommands(unsigned char commands[])
{
  unsigned char nextByte;
  //unsigned char nextChar;
  short value;
  double objectT=0;
  char output[16]; //buffer for temperator

  int * pCount;
  int iCount=0;
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
        moveTemperatorSensorOut(); //temporary set here. donn't know where is is the right place.
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
      }
      else if(nextByte==3)
      {
        //3 put shenchou to plate
        doPutShenChouToPlate(value);				
      }
      else if(nextByte==4)
      {
        //4 put water to plate
        doPutWaterToPlate(value);				
      }
      else if(nextByte==5)
      {
        //5   put jiu to plate
        doPutJiuToPlate(value);					
      }
      else if(nextByte==6)
      {
        // 6 put mayou to plate
        doPutMaYouToPlate(value);					
      }
      else if(nextByte==7)
      {
        //  7 put qianfeng to plate
        doPutQianFengToPlate(value);				
      }
      else if(nextByte==100)
      {
        //take salt x turns. e.g. 100 take salt 2g
        doTakeSalt(value);				
      }
      else if(nextByte==101)
      {
        //101 take sugar x turns
        doTakeSugar(value);				
      }
      else if(nextByte==102)
      {
        // 102 take weijing x turns 
        doTakeWeiJing(value);				
      }
      else if(nextByte==103)
      {

        // take group of ingredients (e.g. take salt x turns and take sugar x turns and take weijing x turns). 103(100 x turns 102 xturns 103 xturns)
        //nextByte=GetNextCommand(commands, pCount); //this shoud be '('
        nextByte=GetNextCommand(commands, pCount); //this shoud be 'B'
        while(nextByte!=')') //')' is the end char of group
        {						
          nextByte=GetNextCommand(commands, pCount); //this should be 100(salt), 101(sugar), 102(weijing)
          value=GetNextValue(commands, pCount); //this should be turns
          if(nextByte==100)
          {	
            motorSalt.steps=value;

          }
          else if(nextByte==101)
          {	
            motorSugar.steps=-value;
          }
          else if(nextByte==102)
          {		
            motorWeijing.steps=-value;
          }
          nextByte=GetNextCommand(commands, pCount); //this shoud be 'B'. if it is ')' then it is end of group 
        }	//end while(nextByte!=')')
        
        //motorGroup1.driveMotors(ingredientMotorsGroup, 3);
        
        motorSalt.steps=200;
        motorSugar.steps=-200;
        motorWeijing.steps=200;
        Motor motors[3];    
    
	motors[0]=motorSalt;
        motors[1]=motorSugar;
        motors[2]=motorWeijing;
	motorGroup1.driveMotors(motors, 3);

      }
      break;
    case 'C':
      //dispay text in lcd
      nextByte=GetNextCommand(commands, pCount);
      if(nextByte==1)
      {
        //1. put food into plate and request user to comfirm
        displayPutFootInCookingPlateLcd(); 
        doRequestUserToConfirm();  
        displayBusyLcd();      
      }   
      else if(nextByte==2)
      {
        //2. busy message
        displayBusyLcd();                          
      }     
      else if(nextByte==3)
      {
        //3. ask for stiring and request user to comfirm
        displayAskStiringPlateLcd();  
        doRequestUserToConfirm();
        displayBusyLcd();        
      } 
      else if(nextByte==4)
      {
        //4. done message and request user to comfirm
         setMotorToStartPosition();
        displayCookingDone(); 
        doRequestUserToConfirm();        
      }  
    else if(nextByte==5)
      {
        //5. open cover and request user to comfirm
        displayOpenCover();  
        doRequestUserToConfirm();
        displayBusyLcd();
      }  
    else if(nextByte==6)
      {
        //6. close cover and request user to comfirm
        displayCloseCover(); 
        doRequestUserToConfirm(); 
        displayBusyLcd();
      }     
   else if(nextByte==7)
      {
        //7. put oil into plate and request user to comfirm
        displayPutOilLcd();  
        doRequestUserToConfirm();
        displayBusyLcd();
      }      	
   else if(nextByte==8)
      {
        //8. put ingredients into plate and request user to comfirm
        displayPutIngredientsLcd();  
        doRequestUserToConfirm();
        displayBusyLcd();
      }      			
      break;
      
    case 'D':
      //request use to handel message
      //waiting for press ok button by user
      doRequestUserToConfirm();
      break;
    case 'E':
      //
      //
      break;
    case 'F':
      //F.cooking x sconds
      displayBusyLcd();
      //moveTemperatorSensorOut();
      
      value=GetNextValue(commands, pCount);
      for(int icount=0; icount<value; icount++)
      {  
        dtostrf(mlx.readAmbientTempC(),2,2,output); 
        displayLineInLcd(1,  output);
      
        dtostrf(mlx.readObjectTempC(),2,2,output);  
        displayLineInLcd(2,  output);    
        delay(1000);    //delay(value*1000);  
      }
      
      //moveTemperatorSensorIn();
      displayBusyLcd();
      
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
      displayBusyLcd();  
      //moveTemperatorSensorOut();
      
      value=GetNextValue(commands, pCount);
      objectT=mlx.readObjectTempC();
      //inox 50; iron 85
      _requestHandled=false; // for test purpose
      while(objectT<value && !_requestHandled) //while(objectT<value)
      {
        //Serial.print("*C\tObject = "); Serial.print(objectT); Serial.println("*C");
       delay(500);
       objectT=mlx.readObjectTempC();
        
       dtostrf(mlx.readAmbientTempC(),2,2,output); 
       displayLineInLcd(1,  output);      
      
       dtostrf(mlx.readObjectTempC(),2,2,output);  
       displayLineInLcd(2,  output);        
      }
      
      //moveTemperatorSensorOut();
      
      displayBusyLcd();
      
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




//value is in seconds
void doPutOilToPlate(int value)
{
  moveOilOut();
  connectRelayPin(pinOil, value*1000);  
  moveOilIn();
}

//value is in seconds
void doPutLaoChouToPlate(int value)
{
  moveOilOut();
  connectRelayPin(pinLaoChou, value*1000); 
  moveOilIn(); 
}

//value is in seconds
void doPutShenChouToPlate(int value)
{
   moveOilOut();
  connectRelayPin(pinShenChou, value*1000); 
  moveOilIn();  
}

//value is in seconds
void doPutWaterToPlate(int value)
{
   moveOilOut();
  connectRelayPin(pinWater, value*1000);  
   moveOilIn(); 
}

//value is in seconds
void doPutJiuToPlate(int value)
{
   moveOilOut();
  connectRelayPin(pinJiu, value*1000); 
  moveOilIn();  
}

//value is in seconds
void doPutMaYouToPlate(int value)
{
   moveOilOut();
  connectRelayPin(pinMaYou, value*1000); 
  moveOilIn();  
}

//value is in seconds
void doPutQianFengToPlate(int value)
{
   moveOilOut();
  connectRelayPin(pinQianFeng, value*1000); 
  moveOilIn();  
}

void doTakeSalt(int turns){   

  //positive clockwise   
  //512 is one turn(measured by practice)
  // shiftMotor.goSteps(4096, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
  //shiftMotor.goSteps(turns, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
  //shiftMotor.sleepMotor();       
  motorGroup1.driveMotors(motorSalt, turns);  
  //delay(1000); //maybe can be removed
}

void doTakeSugar(int turns){ 
  //shiftMotor.goSteps(turns, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
  //shiftMotor.sleepMotor();   
  //delay(1000);
   turns=turns *-1;
   motorGroup1.driveMotors(motorSugar, turns);  
}

void doTakeWeiJing(int turns){ 
  //shiftMotor.goSteps(turns, false, 0, 1, 2, 3, 1, motorSpeed28, noStopPin);
  //shiftMotor.sleepMotor();   
  
  //delay(1000);
  // turns=turns *-1;
   motorGroup1.driveMotors(motorWeijing, turns);  
}

void moveTotal()
{
  //always for its start postition
  //turn back
  motorTurnTotal.stopPin=turnPinTotal;
  motorGroup1.driveMotors(motorTurnTotal, 500); 
  delay(1000);

  //move back
  motorMoveTotal.stopPin=stopPinTotal;
  motorGroup2.driveMotors(motorMoveTotal, -500); 
  delay(1000);
  //end always for its start postition
  
  //move
  motorMoveTotal.stopPin=NO_STOP_PIN;
  motorGroup2.driveMotors(motorMoveTotal, 220);
  delay(1000);

  //turn
  motorTurnTotal.stopPin=NO_STOP_PIN;
  motorGroup1.driveMotors(motorTurnTotal, -330);
  delay(1000);

  //turn back
  motorTurnTotal.stopPin=turnPinTotal;
  motorGroup1.driveMotors(motorTurnTotal, 500); 
  delay(1000);

  //move back
  motorMoveTotal.stopPin=stopPinTotal;
  motorGroup2.driveMotors(motorMoveTotal, -500); 
  delay(1000);
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

void displayPutIngredientsLcd()
{
  LCDA.CLEAR();
  displayLineInLcd(0, "put ingredients");    
  currentIndex=0;    
}

void displayPutOilLcd()
{
  LCDA.CLEAR();
  displayLineInLcd(0, "put oil");    
  currentIndex=0;    
}

void displayOpenCover()
{
  LCDA.CLEAR();
  displayLineInLcd(0, "open cover");    
  currentIndex=0;    
}

void displayCloseCover()
{
  LCDA.CLEAR();
  displayLineInLcd(0, "close cover");    
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
  //connectRelayPin(pinBusyLed);  
  digitalWrite(pinBusyLed, HIGH);
}

void doSetBusySignOff()
{
  //disconnectRelayPin(pinBusyLed); 
  digitalWrite(pinBusyLed, LOW);
}

void doSetIdleSignOn()
{
  //connectRelayPin(pinIdleLed);  
  digitalWrite(pinIdleLed, HIGH);
}

void doSetIdleSignOff()
{
  //connectRelayPin(pinIdleLed); 
  digitalWrite(pinIdleLed, LOW); 
}

void doRequestUserToConfirm()
{
    _requestHandled=false;				
    while(!_requestHandled)
    {
      delay(100);
      doSetBusySignOff();
      delay(100);
      doSetBusySignOn();
    }
    doSetBusySignOn();
}

void doOpenCover()
{
    digitalWrite(openCoverMotorPin1, HIGH);
    digitalWrite(openCoverMotorPin2, LOW);
    delay(5000);
    
    digitalWrite(openCoverMotorPin1, LOW);
    digitalWrite(openCoverMotorPin2, LOW);
    /*
  while(digitalRead(stopPinOpenCover)==HIGH)
  {
    digitalWrite(openCoverMotorPin1, LOW);
    digitalWrite(openCoverMotorPin2, HIGH);
    delay(100);
  }
  */
}

void doCloseCover()
{
    digitalWrite(openCoverMotorPin1, LOW);
    digitalWrite(openCoverMotorPin2, HIGH);
    
    delay(5000);
    
    digitalWrite(openCoverMotorPin1, LOW);
    digitalWrite(openCoverMotorPin2, LOW);
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

