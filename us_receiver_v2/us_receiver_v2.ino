#include "DualVNH5019MotorShield.h"
#define OUTPUT_INTERVAL 1000
#define SIGNAL_GAP 250
#define RELAX_DELAY 10

long encoderThreshold = 1000;
volatile long lastFrontTime = 0;
volatile int signalsReceived = 0;
long IOTimer = 0;
String message = "";
volatile bool messageReady = false;
bool start = false;
bool finish = false;
String completeMessage = "";
volatile long encoderCounter = 0;
long LEDTimer = 0;
bool LED = false;
bool LEDflag = true;
bool over = false;
String times = "";
volatile int noise = false;
//DualVNH5019MotorShield motor;

void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(A0, INPUT);
  pinMode(49, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello");
  attachInterrupt(digitalPinToInterrupt(2), onHigh, FALLING);
  attachInterrupt(digitalPinToInterrupt(18), encoderInterrupt, RISING);
//  motor.init();
}

void onHigh() {
  long lft = millis() - lastFrontTime;
  times += String(lft) + " ";
  if (lft < 0.5*SIGNAL_GAP) {
    noise++;
  } else {
   if ((lft > 1.5*SIGNAL_GAP)) {
    signalsReceived += 1;
    message.concat('0');
  } else if ((lft > SIGNAL_GAP)) {
    signalsReceived += 1;
    message.concat('1');
  } else if((lft > (0.5*SIGNAL_GAP))) {
    messageReady = true;
  }
  lastFrontTime = millis();
      delayMicroseconds(RELAX_DELAY*1000);
  }
  
}


void showMessage() {
  char symbol = 0;
  for (int sym = 0; sym < message.length(); sym++) {
    char bt = message.charAt(7-sym);
    if (bt == '1') {
      symbol |= (1 << sym);
    } else {
      symbol |= (0 << sym);
    }
  }
  
  if (start) {
    Serial.print(symbol);
    completeMessage += symbol;
  }
  
  if (symbol == '\n') {
    if (start && !over) {
      finish = true;
      over = true;
    }
    start = true;
  }
  message = "";
}

void startMotor(int dir) {
  encoderCounter = 0;
//  motor.setM2Speed(100 * dir);
}

void encoderInterrupt() {
  encoderCounter++;

  if (encoderCounter > encoderThreshold) {
//    motor.setM2Speed(0);
  }
}

void loop() {

//  Serial.println(millis() - lastFrontTime);

  
  
if ((millis() - IOTimer) > OUTPUT_INTERVAL) {
   Serial.print(signalsReceived);
   Serial.print(" ");
   Serial.print(noise);
   Serial.print(" ");
  Serial.println(times + "/" + String(SIGNAL_GAP) + String(" ")
                             + String(1.5*SIGNAL_GAP) + String(" ")
                             + String(0.5*SIGNAL_GAP) + String(" "));
  noise = 0;                           
  times = "";
//   times = "";
//   Serial.println("Spikes" + String(spikes));
   IOTimer = millis();
  }
  if (messageReady) {
    showMessage();
    messageReady = false;
  }
  if (finish) {
    startMotor(1); 
    Serial.println("Message: " + completeMessage);
    finish = false;
    LED = true;
    LEDflag = false;
  }

  if (Serial.available()) {
    String command = Serial.readString();
    if (command.charAt(0) == 's') {
      String numString = "";
      for (int i = 2; i < command.length(); i++) {
        numString += command.charAt(i);
      }
      encoderThreshold = 131 * 16 * numString.toInt();
      Serial.println("ET=" + encoderThreshold);
    }
    if (command.charAt(0) == 'f') {
      encoderCounter = 0;
      startMotor(1);
      Serial.println("forward");
    }
    if (command.charAt(0) == 'b') {
      encoderCounter = 0;
      startMotor(-1);
      Serial.println("backward");
    }
  }

  if (LEDflag) {
    if (!start) {
      if (millis() - LEDTimer > 3000) {
        LED = !LED;
        LEDTimer = millis();
      }
    }
  
    if (start) {
      if (millis() - LEDTimer > 1000) {
        LED = !LED;
        LEDTimer = millis();
      }
    }
  }
  digitalWrite(49, LED);
  

  
}
