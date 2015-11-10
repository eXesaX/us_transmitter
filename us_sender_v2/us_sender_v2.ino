#define TIME 12
#define SIGNAL_GAP 130

void setup() {
  DDRB = B00000011;
  //pinMode(8, OUTPUT);
}

void sendBits(String bits) {
  for (int i = 0; i < bits.length(); i++) {
    if (bits.charAt(i) == '1') {
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delay(SIGNAL_GAP);  
    } else {
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delay(1.4*SIGNAL_GAP);  
    }
    if (i == bits.length() - 1) {
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
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
  for (int num = 0; num < 255; num++) {
    sendBits(String(num, BIN));  
  }
  
}
