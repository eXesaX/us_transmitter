#define OUTPUT_INTERVAL 1000
#define SIGNALS_GAP 300

volatile long acc = 0;
volatile long lastFrontTime = 0;
volatile int signalsReceived = 0;
long IOTimer = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(2), onHigh, RISING);
}

void onHigh() {
  acc += 1;
  if ((millis() - lastFrontTime > SIGNALS_GAP)) {
    signalsReceived += 1;
    acc = 0;
  }
  lastFrontTime = millis();
  
}

void loop() {
  if ((millis() - IOTimer) > OUTPUT_INTERVAL) {
    Serial.println(signalsReceived);
    IOTimer = millis();
  }
  
  
}
