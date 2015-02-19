/*
 David Spruill, 2014
 
 This code is designed to control the Bblr Bubble Display made as a capstone final project 
 in Duke's ECE 459 Embedded Systems course taught by Dr. John Board.  
 
 This code currently deals with both time keeping, bluetooth phone connections for picture
 reception, and bubble (pixel) compensation planning.
 */

#include <TimerOne.h>
#include <Time.h>

//Image
#define rails 24 // number of rails
#define height 5 // maximum number of bubbles that can fit in the board
#define charHeight 5
#define charWidth 3
#define allowedHeight 100//max allowed height of a schedule

//Timing
#define pixelPitch 330 // distance between the pixels
#define blockPitch 1000 // distance between letters/blocks
#define compensation 10 // compensation time
#define pixelDuration 8
#define initialDelay 3000
#define calibrationBubbleSize 40

boolean PixelDelay[rails] = {
  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

int counterOffset = 0;
byte mode = 0;
boolean currTime = 0;
boolean phoneSetTime = false;

int currRails = 24;
int currRowLimit = 5;

boolean setSchedule = false;
boolean startPrint = false;

boolean schedule[rails][allowedHeight];

unsigned long lastWrite = 0;
int currRow = 0;

// Alphanumeric lib
boolean alphabet[26][3][5] = {
    {{1, 1, 1, 1, 1},{1, 0, 1, 0, 0},{1, 1, 1, 1, 1}},  //A
    {{1, 1, 1, 1, 1},{1, 0, 1, 0, 1},{0, 1, 0, 1, 0}},  //B
    {{0, 1, 1, 1, 0},{1, 0, 0, 0, 1},{1, 0, 0, 0, 1}},  //C
    {{1, 1, 1, 1, 1},{1, 0, 0, 0, 1},{0, 1, 1, 1, 0}},  //D
    {{1, 1, 1, 1, 1},{1, 0, 1, 0, 1},{1, 0, 0, 0, 1}},  //E
    {{1, 1, 1, 1, 1},{1, 0, 1, 0, 0},{1, 0, 0, 0, 0}},  //F
    {{0, 1, 1, 1, 0},{1, 0, 1, 0, 1},{1, 0, 0, 1, 0}},  //G ~ looks funny
    {{1, 1, 1, 1, 1},{0, 0, 1, 0, 0},{1, 1, 1, 1, 1}},  //H
    {{1, 0, 0, 0, 1},{1, 1, 1, 1, 1},{1, 0, 0, 0, 1}},  //I
    {{0, 0, 0, 1, 0},{1, 0, 0, 0, 1},{1, 1, 1, 1, 0}},  //J
    {{1, 1, 1, 1, 1},{0, 1, 0, 1, 0},{1, 0, 0, 0, 1}},  //K
    {{1, 1, 1, 1, 1},{0, 0, 0, 0, 1},{0, 0, 0, 0, 1}},  //L
    {{1, 1, 1, 1, 1},{0, 1, 1, 0, 0},{1, 1, 1, 1, 1}},  //M
    {{1, 1, 1, 1, 1},{0, 1, 1, 1, 0},{1, 1, 1, 1, 1}},  //N
    {{0, 1, 1, 1, 0},{1, 0, 0, 0, 1},{0, 1, 1, 1, 0}},  //O
    {{1, 1, 1, 1, 1},{1, 0, 1, 0, 0},{0, 1, 0, 0, 0}},  //P
    {{0, 1, 1, 1, 0},{1, 0, 0, 1, 1},{0, 1, 1, 1, 1}},  //Q ~ good be better
    {{1, 1, 1, 1, 1},{1, 0, 1, 1, 0},{0, 1, 0, 0, 1}},  //R
    {{0, 1, 0, 0, 1},{1, 0, 1, 0, 1},{1, 0, 0, 1, 0}},  //S
    {{1, 0, 0, 0, 0},{1, 1, 1, 1, 1},{1, 0, 0, 0, 0}},  //T
    {{1, 1, 1, 1, 0},{0, 0, 0, 0, 1},{1, 1, 1, 1, 0}},  //U
    {{1, 1, 1, 0, 0},{0, 0, 0, 1, 1},{1, 1, 1, 0, 0}},  //V
    {{1, 1, 1, 1, 1},{0, 0, 0, 1, 0},{1, 1, 1, 1, 1}},  //W
    {{1, 1, 0, 1, 1},{0, 0, 1, 0, 0},{1, 1, 0, 1, 1}},  //X
    {{1, 1, 0, 0, 0},{0, 0, 1, 1, 1},{1, 1, 0, 0, 0}},  //Y
    {{1, 0, 0, 1, 1},{1, 0, 1, 0, 1},{1, 1, 0, 0, 1}}}; //Z

boolean numbers[10][3][5] =  {
    {{0, 1, 1, 1, 0},{1, 0, 0, 0, 1},{0, 1, 1, 1, 0}},  //0
    {{0, 1, 0, 0, 1},{1, 1, 1, 1, 1},{0, 0, 0, 0, 1}},  //1
    {{1, 0, 1, 1, 1},{1, 0, 1, 0, 1},{1, 1, 1, 0, 1}},  //2
    {{1, 0, 0, 0, 1},{1, 0, 1, 0, 1},{1, 1, 1, 1, 1}},  //3
    {{1, 1, 1, 0, 0},{0, 0, 1, 0, 0},{1, 1, 1, 1, 1}},  //4
    {{1, 1, 1, 0, 0},{1, 0, 1, 0, 1},{1, 0, 1, 1, 1}},  //5
    {{1, 1, 1, 1, 1},{1, 0, 1, 0, 1},{1, 0, 1, 1, 1}},  //6
    {{1, 0, 0, 1, 1},{1, 0, 1, 0, 0},{1, 1, 0, 0, 0}},  //7
    {{1, 1, 1, 1, 1},{1, 0, 1, 0, 1},{1, 1, 1, 1, 1}},  //8
    {{1, 1, 1, 0, 0},{1, 0, 1, 0, 0},{1, 1, 1, 1, 1}}}; //9    These last several could use some straightening out

void setup()
{
  pinMode(13,OUTPUT);
  Serial.begin(9600);  // Begin the serial monitor at 9600bps

  Serial1.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  Serial1.print("$");  // Print three times individually
  Serial1.print("$");
  Serial1.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD

  Serial.println("configuring");
  Serial1.println("U,9600,E");  // Change the baudrate to 9600, even parity
  Serial1.println("---");
  Serial1.end();
  Serial1.begin(9600);  // Start bluetooth serial at 9600

  Timer1.initialize(1000000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerInt ); // attach the service routine here
  setTime(0);
}

void loop()
{
  if(Serial1.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    char currByte = Serial1.read();
    switch(currByte){
    case 0xFE:
      updateTime();
      break; 
    case 0xFD:
      getImage();
      break;
    case 0xFC:
      stopDisplay();
      break;
    case 0xFB:
      startClock();
      break;
    case 0xFA:
      getText();
      break;
    }
  }
  if(startPrint){
    //begin printing
    if(millis() - lastWrite > pixelPitch){
      for(int i = 0; i < rails; i++){
        Serial.print(String(schedule[i][currRow],DEC));
         //here we would turn on the solenoids 
      }
      Serial.print("\n");
      delay(pixelDuration);
      for(int i =0; i < rails; i++){
         //here we would turn off the solenoids 
      }
      lastWrite = millis();
      currRow++;
      if(currRow >= currRowLimit) {
        stopDisplay(); // appropriately stop printing & clear schedule
        currRow = 0; 
        Serial.print("\n");
      }
    }
    else {
      for(int i = 0; i < rails; i++){
        //Serial.print("0"); //this else isn't actually needed, just for testing
      } 
      
    }
  }
}

void getText(){
  
}

void getImage() {
  digitalWrite(13,HIGH);
  char received[10] = {
    0,0,0,0,0,0,0,0,0,0          };
  int receivedCount = Serial1.readBytesUntil(0xA0,received,10);
  if(receivedCount < 2) return;
  
  int imageRails = received[0];
  if(imageRails > rails) return;
  
  currRowLimit = received[1];
  Serial.println("rails: "+String(imageRails) + " rows: "+String(currRowLimit));
  char *image = new char[imageRails*currRowLimit+10];
  int pixelsReceived = Serial1.readBytesUntil(0xFF,image,imageRails*currRowLimit);
  Serial.print("Data: ");
  for(int i = 0; i < imageRails*currRowLimit+10; i++){Serial.print(String(image[i],DEC) + " "); if(i%(imageRails)==0)Serial.print("\n");}
  Serial.println();
  
  int leftSide = (rails - imageRails) / 2;  if(leftSide < 0) leftSide = 0;
  
  for(int i = 0; i < rails; i++){
    for(int j = 0; j < currRowLimit; j++){
     if(i>=leftSide&&i<(leftSide+imageRails)){
      schedule[i][j] = image[(i-leftSide)+j*(imageRails)-8] == 0x01; 
     }
     else schedule[i][j] = 0;
    } 
  }
  startPrint = true;
  setSchedule = true;
}

void stopDisplay() { //stop printing and clear the schedule
  startPrint = false;
  for(int i = 0; i < rails; i++){
   for(int j = 0; j < allowedHeight; j++){
    schedule[i][j] = 0;
   } 
  }
}

void startClock() {

}

void updateTime(){
  while(Serial1.available() < 5) {
  }
  char received[10] = {
    0,0,0,0,0,0,0,0,0,0          };
  int receivedCount = Serial1.readBytesUntil(0xFF,received,10);
  Serial.println();
  setTime((unsigned char)received[0],(unsigned char)received[1],(unsigned char)received[2],
  (unsigned char)received[3],(unsigned char)received[4],(word)(((unsigned) received[5])<<8) + (word)((unsigned)received[6]));
  phoneSetTime = true;
}

void timerInt()
{
  if(++counterOffset == 30 & !startPrint){
    counterOffset = 0;
    if(phoneSetTime){
      digitalWrite(13,digitalRead(13)^1); 
      if(mode == 0){
        if(currTime)  WriteCurrTime();
        if(!currTime) WriteCurrDate();
        currTime = !currTime;
      }
    }
  }
}

void WriteCurrTime(){
  currRowLimit = 5;
  if(rails < 24) return;
  for(int i = 0; i < currRowLimit; i++){
     int currHour = hour();
     char AorP = 'A';
     if(currHour > 12) {currHour-=12; AorP = 'P';}
     schedule[0][i]  = numbers[currHour/10][0][i];
     schedule[1][i]  = numbers[currHour/10][1][i];
     schedule[2][i]  = numbers[currHour/10][2][i];
     
     schedule[3][i]  = false;
     
     schedule[4][i]  = numbers[currHour%10][0][i];
     schedule[5][i]  = numbers[currHour%10][1][i];
     schedule[6][i]  = numbers[currHour%10][2][i];
     
     schedule[7][i]  = (i%2)-1==0 && i>0; //colons
     
     schedule[8][i]  = numbers[minute()/10][0][i];
     schedule[9][i]  = numbers[minute()/10][1][i];
     schedule[10][i] = numbers[minute()/10][2][i];
     
     schedule[11][i] = false;
     
     schedule[12][i] = numbers[minute()%10][0][i];
     schedule[13][i] = numbers[minute()%10][1][i];
     schedule[14][i] = numbers[minute()%10][2][i];
     
     schedule[15][i] = false;
     
     schedule[16][i] = alphabet[AorP-65][0][i];
     schedule[17][i] = alphabet[AorP-65][1][i];
     schedule[18][i] = alphabet[AorP-65][2][i];
     
     schedule[19][i] = false;
     
     schedule[20][i] = alphabet['M'-65][0][i];
     schedule[21][i] = alphabet['M'-65][1][i];
     schedule[22][i] = alphabet['M'-65][2][i];
     
     schedule[23][i] = false; 
  }
  startPrint = true;
  setSchedule = true;
}
void WriteCurrDate(){
  currRowLimit = 5;
  if(rails < 24) return;
  for(int i = 0; i < currRowLimit; i++){
     schedule[0][i]  = numbers[month()/10][0][i];
     schedule[1][i]  = numbers[month()/10][1][i];
     schedule[2][i]  = numbers[month()/10][2][i];
     
     schedule[3][i]  = false;
     
     schedule[4][i]  = numbers[month()%10][0][i];
     schedule[5][i]  = numbers[month()%10][1][i];
     schedule[6][i]  = numbers[month()%10][2][i];
     
     schedule[7][i]  = false;
     
     schedule[8][i]  = numbers[day()/10][0][i];
     schedule[9][i]  = numbers[day()/10][1][i];
     schedule[10][i] = numbers[day()/10][2][i];
     
     schedule[11][i] = false;
     
     schedule[12][i] = numbers[day()%10][0][i];
     schedule[13][i] = numbers[day()%10][1][i];
     schedule[14][i] = numbers[day()%10][2][i];
     
     schedule[15][i] = false;
     
     schedule[16][i] = numbers[(year()%100)/10][0][i];
     schedule[17][i] = numbers[(year()%100)/10][1][i];
     schedule[18][i] = numbers[(year()%100)/10][2][i];
     
     schedule[19][i] = false;
     
     schedule[20][i] = numbers[(year()%100)%10][0][i];
     schedule[21][i] = numbers[(year()%100)%10][1][i];
     schedule[22][i] = numbers[(year()%100)%10][2][i];
     
     schedule[23][i] = false; 
  }
  startPrint = true;
  setSchedule = true;
}




