int pixelDuration=10;
int railA;
int AveragedReadings[4];
int ReadingNum=0;
int sensorPin[2];
long CalibratedQTIReading[2];
long QtiBubbleScaling=10;

long result = 0;
int BubbleSeen;
int BubbleLength=0;
int RCBubbleReading;

int ReadingsFinished=0;
int tempSum=0;
void setup()                    // run once, when the sketch starts
{
   
   Serial.begin(9600);
   Serial.println("start");      // a personal quirk 
   sensorPin[0]= A3;              // 220 or 1k resistor connected to this pin
   //sensorPin[1]=A4;
   railA = 9;
   
   pinMode(railA, OUTPUT);
   digitalWrite(railA, LOW);
   
   CalibratedQTIReading[0] = getCalibratedValue(sensorPin[0]);
   
   Serial.print("CalibratedQTIReading[0] = "); 
   Serial.print(CalibratedQTIReading[0]); 
   Serial.println(); 
   Serial.print("----------------MyPixelDuration is : ");
   Serial.print(pixelDuration);
   Serial.println(); 
   Serial.println(); 

   // release 5 bubbles
   for(int i = 0; i<=4; i++){
     delay(1000);
     releaseBubble(railA, pixelDuration);
   }

}

void loop()                     // run over and over again
{
  if(!ReadingsFinished){
    getBubbleReading(sensorPin[0],CalibratedQTIReading[0]);
  }
  else{
    ReadingNum=0;
    ReadingsFinished=0;
    
    for(int i = 1; i<=4; i++){
      tempSum += AveragedReadings[i];
    }
    
    
    tempSum = (tempSum/4);
    
    if(tempSum > 12){ 
    //tempSum = tempSum-12;
    pixelDuration=pixelDuration-1;
    
    }
    if(tempSum < 12){
    tempSum = 12-tempSum;  
    pixelDuration=pixelDuration+1;

    }  
    tempSum = 0;
    
    Serial.print("----------------MyNewPixelDuration is : ");
    Serial.print(pixelDuration);
    Serial.println(); 
    Serial.println(); 
    
    for(int i = 0; i<=4; i++){
     delay(1000);
     Serial.println("releasing bubble"); 
     releaseBubble(9, pixelDuration);
    }
    
  }

}


void releaseBubble(int Sol, int BubLength){
  digitalWrite(Sol, HIGH);
  delay(BubLength);
  digitalWrite(Sol, LOW);
  return;
}


void getBubbleReading(int sensPin, long CalibratedQti){
   int RCBubbleReading = RCtime(sensPin);
   if(RCBubbleReading <(CalibratedQti-QtiBubbleScaling) && BubbleSeen == 0){
     Serial.print("Calibration Value: ");
     Serial.print(RCBubbleReading);
     Serial.println();  
     Serial.print("Bubble Length: ");
     Serial.print(BubbleLength);
     Serial.println();  
   AveragedReadings[ReadingNum] = BubbleLength;
     Serial.print("AveragedReadings[ReadingNum]: ");
     Serial.print(AveragedReadings[ReadingNum]);
     Serial.println();  
     Serial.println();  
   BubbleSeen =1;
   BubbleLength = 0;
   ReadingNum++;
     if(ReadingNum == 5){
       ReadingsFinished=1;
     }
   }
   else if(RCBubbleReading > (CalibratedQti-QtiBubbleScaling)){
    BubbleSeen = 0; 
   }
   else{
     BubbleLength++;
   }
   delay(10);
}

long getCalibratedValue(int sensPin){
  
  long myRes=0;
  int windowLength = 20;
  for(int i = 0; i<windowLength; i++){
   myRes += RCtime(sensPin); 
  }
  
  myRes = myRes/windowLength;
  return myRes;
}

long RCtime(int sensPin){
   long result = 0;
   pinMode(sensPin, OUTPUT);       // make pin OUTPUT
   digitalWrite(sensPin, HIGH);    // make pin HIGH to discharge capacitor - study the schematic
   delay(10);                       // wait a  ms to make sure cap is discharged

   pinMode(sensPin, INPUT);        // turn pin into an input and time till pin goes low
   digitalWrite(sensPin, LOW);     // turn pullups off - or it won't work
   while(digitalRead(sensPin)){    // wait for pin to go low
      result++;
   }
   return result;                   // report results   
}   
