#define OUTPUT_INTERVAL 1000
#define SIGNAL_GAP 200
#define EOM_THRESHOLD 1000

volatile long acc = 0;
volatile long lastFrontTime = 0;
volatile int signalsReceived = 0;
long IOTimer = 0;
String message = "";
long lFT = 0;
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
  if ((millis() - lastFrontTime > 1.4*SIGNAL_GAP)) {
    signalsReceived += 1;
    message.concat('0');
//    detectEOM();
  } else if ((millis() - lastFrontTime > SIGNAL_GAP)) {
    signalsReceived += 1;
    message.concat('1');
//    detectEOM();
  } else if((millis() - lastFrontTime > (0.3*SIGNAL_GAP))) {
    showMessage();
  }
  
 
  lastFrontTime = millis();
  
}

void detectEOM() {
//  Serial.println(acc);
  if (acc > EOM_THRESHOLD) {
    showMessage();
  }
  acc = 0;
}

void showMessage() {
//  char lastBit = message.charAt(message.length() - 1);
//  message.remove(message.length() - 1);
//  Serial.println("Message: " + message);
  char str[8];
//  message.toCharArray(str, 8);
//  char symbol = GetBitArrayAsByte(str);
  char symbol = 0;
  for (int sym = 0; sym < message.length(); sym++) {
    char bt = message.charAt(7-sym);
    if (bt == '1') {
      symbol |= (1 << sym);  
    } else {
      symbol |= (0 << sym);
    }
    
  }
  Serial.print(symbol);
//  Serial.println(symbol, BIN);
  message = "";
//  message.concat(lastBit);
}

char GetBitArrayAsByte(const char inputArray[8])
{
    char result = 0;
    for (int idx = 0; idx < 8; ++idx)
    {
        result |= (inputArray[7-idx] << idx);
    }
    return result;
}
void loop() {

//  Serial.println(millis() - lastFrontTime);

  
  
  if ((millis() - IOTimer) > OUTPUT_INTERVAL) {
//    Serial.print(signalsReceived);
//    Serial.println(" " + message);
    IOTimer = millis();
  }
  
  
}
