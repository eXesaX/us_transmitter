#include <FastCRC.h>
#include <FastCRC_cpu.h>
#include <FastCRC_tables.h>

#define TIME 15
#define SIGNAL_GAP 300

String message = "";
byte bytes[255];
byte len;
FastCRC8 CRC8;

void setup() {
  DDRB = B00000011;
  //pinMode(8, OUTPUT);
  Serial.begin(115200);
  Serial.println("hello");
} 

void doNSpikes(int n) {
  for (int i = 0; i < n; i++) {
      PORTB = B00000001;
      delayMicroseconds(TIME);
      PORTB = B00000000;
      delayMicroseconds(TIME);
  }
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
  if (Serial.available() > 0) {
    message = Serial.readString();
//    message.remove(message.length());
    message.remove(message.length() - 1);
    message.getBytes(bytes, message.length());
    
    len = message.length() - 1;
    
    Serial.print(message);
    Serial.print('|');
    Serial.println();
    
    for (int i = 0; i < len; i++) {
      Serial.print(i);
      Serial.print(" ");
      Serial.print((char)bytes[i]);
      Serial.print(" ");
      Serial.println(bytes[i]);
    }
    Serial.println();
    
    byte crc = CRC8.smbus(bytes, len);
    
    bytes[len] = crc;
    len++;
    bytes[len] = '\n';
    len++;
   
   for (int i = 0; i < len; i++) {
      Serial.print(i);
      Serial.print(" ");
      Serial.print((char)bytes[i]);
      Serial.print(" ");
      Serial.println(bytes[i]);
    }
    Serial.println();
  } 
  if (message.length() > 0) {
    for (int num = 0; num < len; num++) {
      String bits = String(bytes[num], BIN);
      
      while (bits.length() < 8) {
        bits = "0" + bits;
      }
      Serial.println(bits);

      bits = String(num, BIN) + bits;

      while (bits.length() < 12) {
        bits = "0" + bits;
      }
      Serial.print(bytes[num]);
      Serial.print(" ");
      Serial.println(bits);
      sendBits(bits);
    }
  }
  
  
}
