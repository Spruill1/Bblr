#define pinNum 5
int timings[pinNum];
long lastFired[pinNum];

bool lastReceived[pinNum];
bool bufferFire[pinNum];
bool bufferFilled = false;

#define cycle 500
long startCycle = 0;

#define getArr 0x10
bool needRec = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for(int i=0;i<pinNum;i++){
    timings[i]=200;
    lastReceived[i]=false;
    lastFired[i]=0;
    bufferFire[i]=false;
  }
  
  startCycle = millis();
}

void loop() {
  if(Serial.available()){
   if(!needRec){
    if(Serial.read()==getArr) needRec=true;
   }else{
     
     if(!bufferFilled){
      for(int i=0;i<pinNum;i++){bufferFire[i]=lastReceived[i]}; 
     }
     
     byte incoming = Serial.read();
     for(int i=0;i<pinNum;i++){
       lastReceived[i] = (incoming & 0x80)?true:false;  
       incoming = incoming << 1;
     }
     needRec=false;
   } 
  }
  
  if(millis()-startCycle > cycle){
    //start a cycle, turn on all necessary tubes
    startCycle = millis();
    for(int i-0;i<pinNum;i++){
      if(lastReceive 
    }
    
  }
  
}
