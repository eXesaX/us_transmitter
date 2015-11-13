#define OUTPUT_INTERVAL 1000
#define SIGNAL_GAP 200
#define EOM_THRESHOLD 1000
#define SPIKE_TIME_LOWEST 20
#define SPIKE_TIME_HIGHEST 40
#define RELAX_DELAY 50
#define SPIKES 5
volatile long lastFrontTime = 0;
volatile int signalsReceived = 0;
long IOTimer = 0;
String message = "";
volatile bool messageReady = false;

volatile bool seeking = true;
volatile byte spikes = 0;
volatile long prevSpikeTime = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(2), onHigh, RISING);
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
//  char lastBit = message.charAt(message.length() - 1);
//  message.remove(message.length() - 1);
//  Serial.println("Message: " + message);
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
    Serial.println(signalsReceived);
//    Serial.println(message);
//    Serial.println("Spikes" + String(spikes));
    IOTimer = millis();
  }
  if (messageReady) {
    showMessage();
    messageReady = false;
  }
//  if (spikes > 0) {
//    Serial.println(spikes);
//  }
  
}
