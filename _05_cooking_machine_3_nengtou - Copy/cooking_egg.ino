//cooking
void cookingEgg()
{
   double objectT=0;
   busy=true;
   LCDA.CLEAR();//清屏
   delay(200);
   LCDA.DisplayString(0,0,cookingEggMsg,AR_SIZE(cookingEggMsg));
    
   //turn on
   //turn on cookingplate. relay is default open
   connectRelayPin(pinCookingPlateOnOff);
  
   //switch temperature mode 
   //default is 1200 degree
   connectRelayPin(pinCookingPlateMode);
  
   //raise temprature to 1400 degree
   connectRelayPin(pinCookingPlatePowerPlus);
   delay(3000);
   
   connectRelayPin(pinOil, 5000);
   
   doTakeSalt(660);
   moveTotal();
   
   objectT=mlx.readObjectTempC();
   //inox 50; iron 85
   while(objectT<85)
   {
     Serial.print("*C\tObject = "); Serial.print(objectT); Serial.println("*C");
     delay(3000);
     objectT=mlx.readObjectTempC();
   }
   
   //现在温度达到80度了，再烧1分钟
   delay(10000);
   
   //lower temperature
   // pinMode(pinCookingPlateTemperatureMin, OUTPUT); 
   // digitalWrite(pinCookingPlateTemperatureMin, LOW);
   // delay(200);
   // pinMode(pinCookingPlateTemperatureMin, INPUT);
     
   //turn off
   connectRelayPin(pinCookingPlateOnOff);
   
   LCDA.CLEAR();//清屏
   delay(200);
   //LCDA.DisplayString(0,0,cookingDone,AR_SIZE(cookingDone));
   currentPage=PAGE_COOKING_DONE;
   displayLcd(PAGE_COOKING_DONE, 0);
   
   for(int i=0; i<3; i++)
   {
     playDoneSound();
   }
   busy=false;
}
