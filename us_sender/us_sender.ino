#define TIME 12

void setup() {
  DDRB = B00000011;
  //pinMode(8, OUTPUT);
}

void loop() {
  PORTB = B00000001;
  delayMicroseconds(TIME);
  PORTB = B00000000;
//  delayMicroseconds(TIME);
//  PORTB = B00000001;
//  delayMicroseconds(TIME);
//  PORTB = B00000000;
//  delayMicroseconds(TIME);
//  PORTB = B00000001;
//  delayMicroseconds(TIME);
//  PORTB = B00000000;
//  delayMicroseconds(TIME);
//  PORTB = B00000001;
//  delayMicroseconds(TIME);
//  PORTB = B00000000;
//  delayMicroseconds(TIME);
//  PORTB = B00000001;
//  delayMicroseconds(TIME);
//  PORTB = B00000000;
//  delayMicroseconds(TIME);
//  PORTB = B00000001;
//  delayMicroseconds(TIME);
//  PORTB = B00000000;
//  delayMicroseconds(TIME);
//  PORTB = B00000001;
//  delayMicroseconds(TIME);
//  PORTB = B00000000;
//  delayMicroseconds(TIME);
//  PORTB = B00000001;
//  delayMicroseconds(TIME);
//  PORTB = B00000000;
  delay(500);
}
