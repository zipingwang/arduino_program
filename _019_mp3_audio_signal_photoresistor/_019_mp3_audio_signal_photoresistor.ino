int LDR_Pin = A0; //analog pin 0

int LDR_Pin2 = A1; //analog pin 1 //this as reference

void setup(){
  Serial.begin(9600);
  pinMode(7, OUTPUT);
}

void loop(){
  int LDRReading = analogRead(LDR_Pin); 
  int LDRReading2 = analogRead(LDR_Pin2); 
  if(LDRReading *1.03 <LDRReading2)
    digitalWrite(7, HIGH);
  else
    digitalWrite(7, LOW);
   Serial.println("value");  
  Serial.println(LDRReading);
  Serial.println(LDRReading2);
  delay(100); //just here to slow down the output for easier reading
  
  }
