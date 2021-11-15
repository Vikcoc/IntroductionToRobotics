// circuit pinis
const int latchPin = 11;
const int clockPin = 10;
const int dataPin = 12;

// joystick pins
const int switchPin = 2;
const int xAxisPin = A0;
const int yAxisPin = A1;

// 7 segment pins
const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

// joystick values
int xValue = 0;
int yValue = 0;
bool joyMoved = false;

const int minThreshold= 100;
const int maxThreshold= 900;

// button press values
unsigned long long lastInterrupt = 0;
int interruptCycle = 100;
bool blinking = true;


// dot blinking logic
bool dotOn = false;
unsigned long long switchedTime = 0;
const int blinkInterval = 500;


// display values
int digitsToDisplay[] = {
  0, 0, 0, 0
};

int digitPointToBlink = 3;

int segmentDigits[] = {
  segD4, segD3, segD2, segD1
};
const int segmentsCount = 4;

int digitArray[] = {
//A B C D E F G DP 
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11111101, // 0
  B01100001, // 1
  B11011011, // 2
  B11110011, // 3
  B01100111, // 4
  B10110111, // 5
  B10111111, // 6
  B11100001, // 7
  B11111111, // 8
  B11110111, // 9
};

const int offsetToDot= 10;

void changeBlink() {
  unsigned long long curr = millis();
  if(curr - lastInterrupt > interruptCycle) {
    blinking = !blinking;
    lastInterrupt = curr;
  }
}

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for(int i = 0; i < segmentsCount; i++) {
    pinMode(segmentDigits[i], OUTPUT);
  }

  pinMode(xAxisPin,INPUT);
  pinMode(yAxisPin,INPUT);
  pinMode(switchPin,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(switchPin), changeBlink, FALLING);
  
  digitalWrite(segmentDigits[0], HIGH);
  digitalWrite(segmentDigits[1], HIGH);
  digitalWrite(segmentDigits[2], HIGH);
  digitalWrite(segmentDigits[3], HIGH);
}


bool isDotLit(int digit) {
  if(digit != digitPointToBlink)
    return false;
   if(blinking) {
    unsigned long long currTime = millis();
    if(currTime - switchedTime > blinkInterval) {
      dotOn = !dotOn;
      switchedTime = currTime;
    }
    return dotOn;
   }
   return true;
}

void writeNumber() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(latchPin, LOW);

    int digit = digitsToDisplay[i];
    if(isDotLit(i))
      digit = digit + offsetToDot;
    shiftOut(dataPin, clockPin, MSBFIRST, digitArray[digit]);
    digitalWrite(latchPin, HIGH);
    digitalWrite(segmentDigits[i], LOW);
    delay(5);
    digitalWrite(segmentDigits[i], HIGH);
  }
}

int getJoystickDirection() {
  xValue = analogRead(xAxisPin);
  yValue = analogRead(yAxisPin);

  if(!joyMoved && xValue < minThreshold) {
    joyMoved = true;
    return -2;
  }

  if(!joyMoved && xValue > maxThreshold) {
    joyMoved = true;
    return 2;
  }

  if(!joyMoved && yValue < minThreshold){
    joyMoved = true;
    return -1;
  }

  if(!joyMoved && yValue > maxThreshold) {
    joyMoved = true;
    return 1;
  }

  if(maxThreshold > yValue && minThreshold < yValue && maxThreshold > xValue && minThreshold < xValue) {
    joyMoved = false;
  }
  return 0;
}

void inputLogic(int value) {
  switch(value) {
    case -1: {
      if(blinking && digitPointToBlink > 0)
        digitPointToBlink--;
      break;
    }
    case 1: {
      if(blinking && digitPointToBlink < 3)
        digitPointToBlink++;
      break;
    }
    case -2: {
      if(!blinking) {
        digitsToDisplay[digitPointToBlink]--;
        if(digitsToDisplay[digitPointToBlink] < 0)
          digitsToDisplay[digitPointToBlink] = 9;
      }
      break;
    }
    case 2: {
      if(!blinking) {
        digitsToDisplay[digitPointToBlink]++;
        if(digitsToDisplay[digitPointToBlink] > 9)
          digitsToDisplay[digitPointToBlink] = 0;
      }
      break;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  writeNumber();
  inputLogic(getJoystickDirection());
}
