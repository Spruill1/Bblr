/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
#define railA 2
#define railB 3
#define railC 4
#define layers 5
#define pixelDuration 8
#define pixelPitch 450

String show = "ECE459";

boolean repeat = 0;
boolean first = 1;
int currentIndex = 0;
int currentLayer = 0;

        
boolean alphabet[26][5][3] = {{{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,0,1}},  //A
                             {{1,1,1},{1,0,1},{1,1,0},{1,0,1},{1,1,1}},  //B
                             {{1,1,1},{1,0,0},{1,0,0},{1,0,0},{1,1,1}},  //C
                             {{1,1,0},{1,0,1},{1,0,1},{1,0,1},{1,1,0}},  //D
                             {{1,1,1},{1,0,0},{1,1,0},{1,0,0},{1,1,1}},  //E
                             {{1,1,1},{1,0,0},{1,1,0},{1,0,0},{1,0,0}},  //F
                             {{1,1,1},{1,0,0},{1,1,1},{1,0,1},{1,1,1}},  //G ~ looks funny
                             {{1,0,1},{1,0,1},{1,1,1},{1,0,1},{1,0,1}},  //H
                             {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{1,1,1}},  //I
                             {{1,1,1},{0,1,0},{0,1,0},{1,1,0},{1,1,0}},  //J
                             {{1,0,1},{1,1,0},{1,0,0},{1,1,0},{1,0,1}},  //K
                             {{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1}},  //L
                             {{1,0,1},{1,1,1},{1,0,1},{1,0,1},{1,0,1}},  //M
                             {{1,0,1},{1,1,1},{1,1,1},{1,1,1},{1,0,1}},  //N
                             {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}},  //O
                             {{1,1,0},{1,0,1},{1,1,0},{1,0,0},{1,0,0}},  //P
                             {{1,1,1},{1,0,1},{1,0,1},{1,1,1},{1,1,1}},  //Q ~ good be better
                             {{1,1,1},{1,0,1},{1,1,0},{1,0,1},{1,0,1}},  //R
                             {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}},  //S
                             {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{0,1,0}},  //T
                             {{1,0,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}},  //U
                             {{1,0,1},{1,0,1},{1,0,1},{0,1,0},{0,1,0}},  //V
                             {{1,0,1},{1,0,1},{1,0,1},{1,1,1},{1,0,1}},  //W
                             {{1,0,1},{1,0,1},{1,0,1},{1,1,1},{1,1,1}},  //X
                             {{1,1,1},{1,0,1},{1,1,0},{1,0,1},{1,1,1}},  //Y
                             {{1,1,1},{1,0,1},{1,1,0},{1,0,1},{1,1,1}}}; //Z
                             
boolean numbers[10][5][3] =  {{{0,1,0},{1,0,1},{1,0,1},{1,0,1},{0,1,0}}, //0
                             {{0,1,0},{1,1,0},{0,1,0},{0,1,0},{1,1,1}},  //1
                             {{1,1,0},{0,0,1},{0,1,0},{1,0,0},{1,1,1}},  //2
                             {{1,1,1},{0,0,1},{0,1,1},{0,0,1},{1,1,1}},  //3
                             {{1,0,1},{1,0,1},{1,1,1},{0,0,1},{0,0,1}},  //4
                             {{1,1,1},{1,0,0},{1,1,0},{0,0,1},{1,1,0}},  //5
                             {{1,1,1},{1,0,0},{1,1,1},{1,0,1},{1,1,1}},  //6
                             {{1,1,1},{0,0,1},{0,1,0},{1,0,0},{1,0,0}},  //7
                             {{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,1,1}},  //8
                             {{1,1,1},{1,0,1},{1,1,1},{0,0,1},{0,1,1}}}; //9    These last several could use some straightening out

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(railA, OUTPUT);
  pinMode(railB, OUTPUT);
  pinMode(railC, OUTPUT);
}

boolean getEncoding(char currentLetter, int currentLayer, int rail){

  if(currentLetter>64 && currentLetter<91) {
    //this is a capital letter
    return alphabet[(currentLetter-65)][currentLayer][rail];
  }
  if(currentLetter>47 && currentLetter<58) {
    //This is a number
    return numbers[(currentLetter-48)][currentLayer][rail];
  }
  if(currentLetter == 32){
    //This is a space
    return 0; 
  }
  return 0;
}

// the loop function runs over and over again forever
void loop() {
  if(repeat||first){
    digitalWrite(railA,getEncoding(show[currentIndex],currentLayer,0));
    digitalWrite(railB,getEncoding(show[currentIndex],currentLayer,1));
    digitalWrite(railC,getEncoding(show[currentIndex],currentLayer,2));
    delay(pixelDuration);
    digitalWrite(railA,0);
    digitalWrite(railB,0);
    digitalWrite(railC,0);
    delay(pixelPitch);
    currentLayer++;
    if(currentLayer>=layers){
      currentLayer = 0;
      currentIndex++;
      delay(750);
    }
    if(currentIndex>=show.length()){
      currentIndex = 0;
      first = 0;
    }
  }
}
