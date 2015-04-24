// Bubble Calibration


int pixelDuration[6];
int sensorPin[6];

int SolenoidTube[6];

int AveragedReadings[6];
long CalibratedQTIReading[6];
int SawBubble[6];

int BubbleLength[6];
int result[6];
int QTIBubbleScaling = 4;
int resultDone[6];
int readAll;
int BubbleEnded; 
int BubbleDone[6];


int refinementCounter=5;
int refineBegin=0;

// Avalanche Game




// Setup Mostly for Calibrate Bubble
void setup(){
  
  //Initialization Code
  BubbleEnded=0;
  for(int i =0; i<6; i++){
   SawBubble[i]=0; 
   AveragedReadings[i]=0;
   BubbleLength[i]=0;
   resultDone[i]=0;
   result[i]=0;
   BubbleDone[i]=0;
   pixelDuration[i]=40;
  
   sensorPin[i]=A0+i;
   SolenoidTube[i]=22+i;
   pinMode(SolenoidTube[i], OUTPUT);      // make pin OUTPUT

  }

  getCalibratedValue(); 
}

void loop(){
  if(Serial.available(){
    byte myReadByte = Serial.read();
    if(myReadByte == 'c'){
      calibrate();
    }
    if(myReadByte == 'g'){
      playGame(); 
    } 
  }
}

  void calibrate(){
  // Count Iteration
  refineBegin++;
  // Release Bubble (For All 24 Tubes; Staggered)
  for(int i = 0; i<6; i++){
     releaseBubble(SolenoidTube[i], pixelDuration[i]);
     Serial.print(i);
     Serial.print(" Bubble Released \n");
     delay(1); // wait 1 millisecond
  }
  // Poll and Get Bubble Size for Each Tube
  
  while(1){
    RCTimeForAllTubes(); //Reads all RC Time Constants once per Tube
    getBubbleLengths();
    
    if(BubbleEnded >= 6){
      Serial.println("I got to the End of Bubble");
      BubbleEnded=0;
      break; 
    }
  // Right now need to find a way to break out of this loop
  // Need to fix that it only reports right size when the next Bubble is seen
  }
  
  // Determine New Pixel Sizes
  for(int i = 0; i<6; i++){
    if(BubbleLength[i]> 30){ 

      pixelDuration[i]=pixelDuration[i]+4; 
    Serial.print(i);
    Serial.print(" pixelDuration++:");
    Serial.print( pixelDuration[i]);
    Serial.print("\n"); 
    }
    if(BubbleLength[i]< 30){ 
          Serial.print(i);
    Serial.print(" pixelDuration--:");
    Serial.print( pixelDuration[i]);
    Serial.print("\n");
      pixelDuration[i]=pixelDuration[i]-4;
    }  
  }
  // Reinitialize everything
   Serial.print("Iteration: ");
   Serial.println(refineBegin);

   for(int i = 0; i<6; i++){
     
    Serial.print(i);
    Serial.print(" BubbleLength:");
    Serial.print(BubbleLength[i]);
    Serial.print("\n");

     BubbleLength[i]= 0;
     
     BubbleDone[i]=0;
   } 
   Serial.print("\n");

  
  
  // Repeat for X times then break
  if(refineBegin == refinementCounter){
    Serial.println("I got to the End of RefineCounter");
    while(1){}; //ENDLESS LOOP TO SIGNIFY THE END  //break;? 
  }
  
}

void getCalibratedValue( ){
  long myRes=0;
  int windowLength = 20;
   
 for(int i = 0; i<6; i++){ 
    pinMode(sensorPin[i], OUTPUT);        // turn pin into an input and time till pin goes low
    digitalWrite(sensorPin[i], HIGH);
  }
 
  for(int i = 0; i<6; i++){ 
    pinMode(sensorPin[i], INPUT);        // turn pin into an input and time till pin goes low
    digitalWrite(sensorPin[i], LOW);     // turn pullups off - or it won't work
  }
  
  // find out how long it takes for sensors to discharge
  readAll=0;
  
  while(1){
    for(int i = 0; i<6; i++){
      if(digitalRead(sensorPin[i])){
        CalibratedQTIReading[i]++;
      } 
      else if(!resultDone[i]){
        resultDone[i]=1;
        readAll++;
      }
    }
    if(readAll == 6){

      for(int i = 0; i<6; i++){
        resultDone[i]=0;
      }
      readAll=0;
      
      break;
    }
  }
 }
   
 // }
/*
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
*/

void releaseBubble(int Sol, int BubLength){
  digitalWrite(Sol, HIGH);
  delay(BubLength);
  digitalWrite(Sol, LOW);
  return;
}

void getBubbleLengths(){
  for(int i = 0; i<6; i++){
    if((result[i]<(CalibratedQTIReading[i] - QTIBubbleScaling)) && SawBubble[i]==0 && BubbleDone[i]==0){
      // Start of a Bubble
      Serial.print(i);
      Serial.println(" result<Calibrated-QTIScaling & Saw Bubble==0 ");
      BubbleLength[i]++;
      SawBubble[i]=1;
    }
    else if((result[i]>=(CalibratedQTIReading[i] - QTIBubbleScaling)) && SawBubble[i]==1 && BubbleDone[i]==0){
      Serial.print(i);
      Serial.println(" result>=Calibrated-QTIScaling & Saw Bubble==1 ");

     // Bubble Ended
     BubbleEnded++;
     BubbleDone[i]=1;
     SawBubble[i]=0; 
    }
    else if(result[i]>=(CalibratedQTIReading[i] - QTIBubbleScaling)){
 
    }
    else if((result[i]<(CalibratedQTIReading[i] - QTIBubbleScaling)) && BubbleDone[i]==0 ){
      Serial.print(" ");

      // Middle of a Bubble
      BubbleLength[i]++;
    }
  }
}

void RCTimeForAllTubes(){
 
  for(int i = 0; i<6; i++){ 
    result[i]=0;
  }
  
  //charge all QTI sensors
  for(int i = 0; i<6; i++){ 
    pinMode(sensorPin[i], OUTPUT);        // turn pin into an input and time till pin goes low
    digitalWrite(sensorPin[i], HIGH);
  }
 
  for(int i = 0; i<6; i++){ 
    pinMode(sensorPin[i], INPUT);        // turn pin into an input and time till pin goes low
    digitalWrite(sensorPin[i], LOW);     // turn pullups off - or it won't work
  }
  
  // find out how long it takes for sensors to discharge
  readAll=0;
  while(1){
    for(int i = 0; i<6; i++){
      if(digitalRead(sensorPin[i])){
        result[i]++;
      } 
      else if(!resultDone[i]){
        resultDone[i]=1;
        readAll++;
      }
    }
    if(readAll == 6){
      for(int i = 0; i<6; i++){
        resultDone[i]=0;
      }
      readAll=0;   
      break;
    }
  }
}  

void getCalibratedValue(){
 
  for(int i = 0; i<6; i++){ 
    CalibratedQTIReading[i]=0;
  }
  
  //charge all QTI sensors
  for(int i = 0; i<6; i++){ 
    pinMode(sensorPin[i], OUTPUT);        // turn pin into an input and time till pin goes low
    digitalWrite(sensorPin[i], HIGH);
  }
 
  for(int i = 0; i<6; i++){ 
    pinMode(sensorPin[i], INPUT);        // turn pin into an input and time till pin goes low
    digitalWrite(sensorPin[i], LOW);     // turn pullups off - or it won't work
  }
  
  // find out how long it takes for sensors to discharge
  readAll=0;
  while(1){
    for(int i = 0; i<6; i++){
      if(digitalRead(sensorPin[i])){
        CalibratedQTIReading[i]++;
      } 
      else if(!resultDone[i]){
        resultDone[i]=1;
        readAll++;
      }
    }
    if(readAll == 6){
      for(int i = 0; i<6; i++){
        resultDone[i]=0;
      }
      readAll=0;   
      break;
    }
  }
}  

