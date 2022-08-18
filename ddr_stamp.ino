#include <BleGamepad.h> 

BleGamepad bleGamepad("DDR Controller");

long thresholds[4] = {1,1,1,1};
String allows[4] = {"down", "right", "up", "left"};
uint8_t dpadCmd[4] = {DPAD_DOWN, DPAD_RIGHT, DPAD_UP, DPAD_LEFT};
bool buttonStat[4] = {false};
long initTimer = 5000;

long weights[4]    = {0,0,0,0};
int  statList[3]   = {0,1,0};
int  readyStats[4] = {0,0,0,0};
int  readStats[4]  = {0,0,0,0};
int  ioList[4]     = {4,0,10,1};

void sendCmd() {
  // down
  if (weights[0] > thresholds[0] && !buttonStat[0]) {
    bleGamepad.press(dpadCmd[0]);
    buttonStat[0] = !buttonStat[0];
    //Serial.println("DN p");
  } else if (weights[0] <= thresholds[0] && buttonStat[0]) {
    bleGamepad.release(dpadCmd[0]);
    buttonStat[0] = !buttonStat[0];
    //Serial.println("DN r");
  }
  // right
  if (weights[1] > thresholds[1] && !buttonStat[1]) {
    bleGamepad.press(dpadCmd[1]);
    buttonStat[1] = !buttonStat[1];
    //Serial.println("RT p");
  } else if (weights[1] <= thresholds[1] && buttonStat[1]) {
    bleGamepad.release(dpadCmd[1]);
    buttonStat[1] = !buttonStat[1];
    //Serial.println("RT r");
  }
  // up
  if (weights[2] > thresholds[2] && !buttonStat[2]) {
    bleGamepad.press(dpadCmd[2]);
    buttonStat[2] = !buttonStat[2];
    //Serial.println("UP p");
  } else if (weights[2] <= thresholds[2] && buttonStat[2]) {
    bleGamepad.release(dpadCmd[2]);
    buttonStat[2] = !buttonStat[2];
    //Serial.println("UP r");
  }
  // left
  if (weights[3] > thresholds[3] && !buttonStat[3]) {
    bleGamepad.press(dpadCmd[3]);
    buttonStat[3] = !buttonStat[3];
    //Serial.println("LT p");
  } else if (weights[3] <= thresholds[3] && buttonStat[3]) {
    bleGamepad.release(dpadCmd[3]);
    buttonStat[3] = !buttonStat[3];
    //Serial.println("LT r");
  }
}  

void serial() {
  while (true) {
    for (int i=0; i<4; i++) {
      while(digitalRead(i+5));
      while(!digitalRead(i+5));
      while(digitalRead(i+5));
      
      for (char j=0; j<24; j++) {
        digitalWrite(ioList[i], 1);
        delayMicroseconds(1);
        digitalWrite(ioList[i], 0);
        delayMicroseconds(1);
        weights[i] = (weights[i] << 1) | (digitalRead(i+5));
      }
      
      weights[i] = weights[i] ^ 0x800000;
    }

    
    // OUTPUT
    if (thresholds[0] == 1) {
      thresholds[0] = weights[0] + 20000; //down
      thresholds[1] = weights[1] + 19000; //right
      thresholds[2] = weights[2] +  1900; //up
      thresholds[3] = weights[3] + 9200; //left
    }
    sendCmd();
    
    //Serial.println(weights[0]);
    for (int j=0; j<4; j++) {
      Serial.printf("%s(%ld/%ld) ", allows[j], weights[j], thresholds[j]);
    }
    Serial.println("");
    weights[0] = weights[1] = weights[2] = weights[3] = 0;
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("Starting BLE work!");
  bleGamepad.begin();
  
  pinMode(4,  OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(0,  OUTPUT);
  pinMode(1,  OUTPUT);
  pinMode(5,  INPUT);
  pinMode(6,  INPUT);
  pinMode(7,  INPUT);
  pinMode(8,  INPUT);

  serial();
}

void loop() {}
