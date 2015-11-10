#define OUTPUT_INTERVAL 1000
#define SIGNAL_GAP 80
#define EOM_THRESHOLD 1000

volatile long acc = 0;
volatile long lastFrontTime = 0;
volatile int signalsReceived = 0;
long IOTimer = 0;
String message = "";

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
  } else if((millis() - lastFrontTime > (0.5*SIGNAL_GAP))) {
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
  message.toCharArray(str, 8);
  char symbol = GetBitArrayAsByte(str);
  Serial.print(symbol);
  message = "";
//  message.concat(lastBit);
}

char GetBitArrayAsByte(const char inputArray[8])
{
    char result = 0;
    for (int idx = 0; idx < 8; ++idx)
    {
        result |= (inputArray[idx] << idx);
    }
    return result;
}
void loop() {
//  if ((millis() - IOTimer) > OUTPUT_INTERVAL) {
//    Serial.print(signalsReceived);
//    Serial.println(" " + message);
//    IOTimer = millis();
//  }
  
  
}
