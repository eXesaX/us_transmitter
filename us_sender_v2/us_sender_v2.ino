#define TIME 15
#define SIGNAL_GAP 250

String message = "ABCDEF";

void setup() {
  DDRB = B00000011;
  //pinMode(8, OUTPUT);
  Serial.begin(115200);
}

void sendBits(String bits) {
  for (int i = 0; i < bits.length(); i++) {
    if (bits.charAt(i) == '1') {
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
      delay(SIGNAL_GAP);  
    } else {
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
      delay(1.4*SIGNAL_GAP);  
    }
    if (i == bits.length() - 1) {
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
      delay(0.5*SIGNAL_GAP);
    }
  }
//  for (int j = 0; j < 5; j++) {
//    PORTB = B00000001;
//    delayMicroseconds(TIME);
//    PORTB = B00000000;
//    delayMicroseconds(TIME);
//  }
  
  
}

void loop() {
  
  for (int num = 0; num < message.length(); num++) {
    String bits = String(message.charAt(num), BIN);
    for (int n = 0; n < 8 - bits.length(); n++) {
      bits = "0" + bits;
    }
    Serial.println(bits);
    sendBits(bits);
  }
//  Serial.println(message);
  
}
