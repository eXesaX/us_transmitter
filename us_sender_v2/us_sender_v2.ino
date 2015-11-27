#define TIME 15
#define SIGNAL_GAP 300

String message = "hello\n";

void doNSpikes(int n) {
  for (int i = 0; i < n; i++) {
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
  }
}


void setup() {
  DDRB = B00000011;
  //pinMode(8, OUTPUT);
  //Serial.begin(115200);
}

void sendBits(String bits) {
  for (int i = 0; i < bits.length(); i++) {
    if (bits.charAt(i) == '1') {
      doNSpikes(3);
      delay(SIGNAL_GAP);  
    } else {
      doNSpikes(3);
      delay(1.5*SIGNAL_GAP);  
    }
    if (i == bits.length() - 1) {
      doNSpikes(3);
      delay(0.5*SIGNAL_GAP);
    }
  }
}

void loop() {
  if (message.length() > 0) {
    for (int num = 0; num < message.length(); num++) {
      String bits = String(message.charAt(num), BIN);
      while (bits.length() < 8) {
        bits = "0" + bits;
      }
      sendBits(bits);
    }
  }
  
  if (Serial.available() > 0) {
    message = Serial.readString();
    Serial.println(message);
  }
  
}
