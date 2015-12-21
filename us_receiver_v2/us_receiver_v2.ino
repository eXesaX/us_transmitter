#include <FastCRC.h>
#include <FastCRC_cpu.h>
#include <FastCRC_tables.h>

#include "DualVNH5019MotorShield.h"
#define OUTPUT_INTERVAL 1000
#define SIGNAL_GAP 250
#define RELAX_DELAY 10
#define TOP_TIME 600

#define DEBUG false

#define WAITING 0
#define RECEIVING 1
#define DONE 2
#define CHEATING 3
bool fin = false;
int mode = 0;
bool motorStarted = false;

long encoderThreshold = 131 * 16 * 20;
volatile long lastFrontTime = 0;
volatile int signalsReceived = 0;
String times = "";
volatile int noise = false;


long IOTimer = 0;

String letter = "";
volatile bool letterReady = false;
const int letterBitLen = 12;

unsigned char completeMessage[255];
int messageLen = 0;

volatile long encoderCounter = 0;

long LEDTimer = 0;
bool LED = false;
bool LEDflag = true;

float topTime = 0;
bool cheat = false;
bool up = false;

DualVNH5019MotorShield motor;
FastCRC8 CRC8;


void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(A0, INPUT);
  pinMode(49, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello");
  attachInterrupt(digitalPinToInterrupt(19), onHigh, FALLING);
  attachInterrupt(digitalPinToInterrupt(18), encoderInterrupt, RISING);
  motor.init();
}

void onHigh() {
  long lft = millis() - lastFrontTime;
  times += String(lft) + " ";
  if (lft < 0.5*SIGNAL_GAP) {
    noise++;
  } else {
   if ((lft > 1.5*SIGNAL_GAP)) {
    signalsReceived += 1;
    letter.concat('0');
  } else if ((lft > SIGNAL_GAP)) {
    signalsReceived += 1;
    letter.concat('1');
  } else if((lft > (0.5*SIGNAL_GAP))) {
    letterReady = true;
  }
  lastFrontTime = millis();
  delayMicroseconds(RELAX_DELAY*1000);
  }
  
}


void checkLetter() {
  int letterNum = 0;
  if (letter.length() == 12) {
    for (int i = 0; i < 4; i++) {
      char lbit = letter.charAt(3-i);
      if (lbit == '1') {
        letterNum |= (1 << i);
      } else {
        letterNum |= (0 << i);
      }
    }

    if (DEBUG) {
      Serial.println("Letter number: " + String(letterNum));
    }

    char symbol = 0;
    String letterBits = letter.substring(4);
    for (int sym = 0; sym < letterBits.length(); sym++) {
      char bt = letterBits.charAt(7-sym);
      if (bt == '1') {
        symbol |= (1 << sym);
      } else {
        symbol |= (0 << sym);
      }
    }

    if (DEBUG) {
      Serial.println("Letter: " + String(symbol));
    }

    completeMessage[letterNum] = symbol;
    
    if (symbol == '\n') {
      messageLen = letterNum + 1;
    }
    mode = RECEIVING;
    
    
  } else {
    if (DEBUG) {
      Serial.println("Wrong letter length: " + String(letter.length()));
    }
  }
  letter = "";
}

void startMotor(int dir) {
  encoderCounter = 0;
  Serial.println("Motor started");
  motor.setM2Speed(300 * dir);
}

void encoderInterrupt() {
  encoderCounter++;

  if (encoderCounter > encoderThreshold) {
    motor.setM2Speed(0);
    if (up) {
      fin = true;
    }
  }
}

byte CRC8_(const byte *data, byte len) {
  byte crc = 0x00;
  while (len--) {
    byte extract = *data++;
    for (byte tempI = 8; tempI; tempI--) {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}

void loop() {
  if ((millis() - IOTimer) > OUTPUT_INTERVAL) {
  
    if (DEBUG) {
       if (cheat) {
        Serial.print("[c]");
       }
       Serial.print(signalsReceived);
       Serial.print(" ");
       Serial.print(noise);
       Serial.print(" ");
    //   Serial.println(times + "/" + String(SIGNAL_GAP) + String(" ")
    //                             + String(1.5*SIGNAL_GAP) + String(" ")
    //                             + String(0.5*SIGNAL_GAP) + String(" "));
       Serial.println(messageLen);
       noise = 0;                           
       times = "";
    
       IOTimer = millis();


       
    }

    if ((messageLen != 0) && (mode != DONE)) {
        mode = RECEIVING;
        byte crc = completeMessage[messageLen - 2];
        if (crc == CRC8.smbus(completeMessage, messageLen - 2)){
          if (DEBUG) {
            Serial.print("crc: ");
            Serial.print(crc, DEC);
            Serial.print("; ");
            Serial.print("CRC: ");
            Serial.println(CRC8.smbus(completeMessage, messageLen - 2), DEC);
          }
          Serial.print("Message: ");
          for (int i = 0; i < messageLen - 2; i++) {
            Serial.print((char)completeMessage[i]);
          }
          Serial.print('\n');
          if (mode != DONE) {
            if(!motorStarted) {
               up = true;
               startMotor(-1);
               mode = DONE;
               motorStarted = true;
            }
           
          }
          
        } else {
          //Serial.println("Wrong CRC");
          if (DEBUG) {
            Serial.print("crc: ");
            Serial.print(crc, DEC);
            Serial.print("; ");
            Serial.print("CRC: ");
            Serial.println(CRC8.smbus(completeMessage, messageLen - 2), DEC);
          }
        }
     }

   }
  
  if (letterReady) {
    checkLetter();
    letterReady = false;
  }
  
  
    
  

  if (Serial.available()) {
    String command = Serial.readString();
    if (command.charAt(0) == 's') {
      String numString = "";
      for (int i = 2; i < command.length(); i++) {
        numString += command.charAt(i);
      }
      encoderThreshold = 131 * 16 * numString.toInt();
      //Serial.println("ET=" + encoderThreshold);
    }
    if (command.charAt(0) == 'f') {
      encoderCounter = 0;
      startMotor(1);
      //Serial.println("forward");
      
    }
    if (command.charAt(0) == 'b') {
      encoderCounter = 0;
      startMotor(-1);
      //Serial.println("backward");
    }

    if (command.charAt(0) == 'c') {
      topTime = millis() / 1000.0;
      cheat = true;
      //Serial.println("Cheat mode");
    if (command.charAt(0) == 'g') {
      for (int b = 0; b < messageLen; b++) {
        Serial.print(completeMessage[b]); 
      }
      Serial.println(" ");
    }
    }
  }

  if (LEDflag) {
    if (mode == WAITING) {
      if (millis() - LEDTimer > 3000) {
        LED = !LED;
        LEDTimer = millis();
      }
    }
  
    if (mode == RECEIVING) {
      if (millis() - LEDTimer > 1000) {
        LED = !LED;
        LEDTimer = millis();
      }
    }
    if (mode == DONE) {
      LED = true;
    }
    if (mode == CHEATING) {
      if (millis() - LEDTimer > 500) {
        LED = !LED;
        LEDTimer = millis();
      }
    }
  }
  digitalWrite(49, LED);

  if (cheat){
    float t = millis() / 1000.0 - topTime;
    //Serial.println(t);
    if ((t) > (TOP_TIME / 2)) {
      mode = CHEATING;
      //Serial.println("fast blinking");
    }
    if ((t) > (TOP_TIME)) {
      mode = DONE;
      up = true;
      startMotor(-1);
      motorStarted = true;
      //Serial.print("up");
    }
  }

  if (fin) {
    delay(10000);
    //Serial.println("Message: abc");
    for (int k = 0; k < messageLen; k++) {
      Serial.print((char)completeMessage[k]);
    }
    Serial.println(" ");
    fin = false;
  }

  
}
