const int redInput = A0;
const int greenInput = A1;
const int blueInput = A2;

const int redOut = 3;
const int greenOut = 5;
const int blueOut = 6;

const int analogInHigh = 1023;
const int analogOutHigh = 255;


int redBrightness;
int greenBrightness;
int blueBrightness;

void setup() {
  pinMode(redInput, INPUT);
  pinMode(greenInput, INPUT);
  pinMode(blueInput, INPUT);

  pinMode(redOut, OUTPUT);
  pinMode(greenOut, OUTPUT);
  pinMode(blueOut, OUTPUT);
}


void loop() {
  redBrightness = map(analogRead(redInput), LOW, analogInHigh, LOW, analogOutHigh);
  greenBrightness = map(analogRead(greenInput), LOW, analogInHigh, LOW, analogOutHigh);
  blueBrightness = map(analogRead(blueInput), LOW, analogInHigh, LOW, analogOutHigh);

  analogWrite(redOut, redBrightness);
  analogWrite(greenOut, greenBrightness);
  analogWrite(blueOut, blueBrightness);
}
