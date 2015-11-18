#include "DualVNH5019MotorShield.h"
#define OUTPUT_INTERVAL 1000
#define SIGNAL_GAP 200
#define SPIKE_TIME_LOWEST 20
#define SPIKE_TIME_HIGHEST 40
#define RELAX_DELAY 50
#define SPIKES 5

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
volatile bool seeking = true;
volatile byte spikes = 0;
volatile long prevSpikeTime = 0;
long LEDTimer = 0;
bool LED = false;
bool LEDflag = true;
bool over = false;
DualVNH5019MotorShield motor;

void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(A0, INPUT);
  pinMode(49, OUTPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(19), onHigh, RISING);
  attachInterrupt(digitalPinToInterrupt(18), encoderInterrupt, RISING);
  motor.init();
}

void onHigh() {
  if (seeking) {
    long spikeTime = micros() - prevSpikeTime;
    if ((spikeTime > SPIKE_TIME_LOWEST) && (spikeTime < SPIKE_TIME_HIGHEST)) {
      spikes += 1;
    } else {
      spikes = 0;
    }
    prevSpikeTime = micros();

    if (spikes == SPIKES) {
      if ((millis() - lastFrontTime > 1.4*SIGNAL_GAP)) {
        signalsReceived += 1;
        message.concat('0');
      } else if ((millis() - lastFrontTime > SIGNAL_GAP)) {
        signalsReceived += 1;
        message.concat('1');
      } else if((millis() - lastFrontTime > (0.3*SIGNAL_GAP))) {
//        showMessage();
        messageReady = true;
      }
      lastFrontTime = millis();
      delay(RELAX_DELAY);
    }
  } else {
    seeking = true;
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
  motor.setM2Speed(100 * dir);
}

void encoderInterrupt() {
  encoderCounter++;

  if (encoderCounter > encoderThreshold) {
    motor.setM2Speed(0);
  }
}

void loop() {

//  Serial.println(millis() - lastFrontTime);

  
  
if ((millis() - IOTimer) > OUTPUT_INTERVAL) {
   Serial.println(signalsReceived);
   Serial.println(message);
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
