//stolen from lab
// declare all the segments pins
const int pinA = 2;
const int pinB = 3;
const int pinC = 5;
const int pinD = 6;
const int pinE = 7;
const int pinF = 8;
const int pinG = 9;
const int pinDP = 4;

const int buzzer = 10;

const int segSize = 8;
const int noOfDigits = 10;

bool dpState = LOW;
int index = 0;


// store the pins in an array for easier access
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};



void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }

  // write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}

//stolen from the internet
const int analogReadHigh = 1023;
const unsigned sample = 300;
unsigned getReading() {
  unsigned val;
  unsigned long long averaging = 0;
  for(int i = 0; i < sample; i++){
   val = analogRead(A0);
   averaging += val;
  }
  val =  averaging / sample;
  return map(val,LOW,analogReadHigh,LOW,noOfDigits);
}

void setup() {
  for (int i = 0; i < segSize; i++) {
  pinMode(segments[i], OUTPUT);
  }
  pinMode(A0, INPUT);
}

const int baseFreq = 2000;
const int freqOffset = 100;
unsigned formerReading = 0;
void loop() {
  unsigned reading = getReading();
  if(reading != formerReading) {
    formerReading = reading;
    displayNumber(reading, LOW);
    tone(buzzer, reading * freqOffset + baseFreq);
  }
}
