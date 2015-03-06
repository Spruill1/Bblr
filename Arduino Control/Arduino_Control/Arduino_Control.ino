#define MSG_init           0x00000001
#define MSG_init_response  0x00000002

#define MSG_init_calibrate 0x00000004

#define MSG_use_solenoids  0x01000000
#define MSG_use_detection  0x02000000
#define MSG_use_leds       0x04000000

#define SEND_ledPos        0x10000000
#define SEND_detection     0x20000000

#define railCount 24
#define MSG_length 4

unsigned long onTime[railCount];  //32-bit unsigned numbers, so up to 4 billion
int varDepth = 1;

bool newMSG = false;
byte currMessage[MSG_length];

unsigned long flags = 0; //allows up to 32 flags

void setup() {
   Serial.begin(9600);
}

void loop() {
  //check for messages from the server
  newMSG = false;
  if(Serial.available() >= MSG_length) {
    //a full message has been recieved
    Serial.read(currMessage, MSG_length);
    newMSG = true;
  }

  
}
