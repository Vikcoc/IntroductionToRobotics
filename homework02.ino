const int btn = 6;
const int carRed = 7;
const int carGreen = 8;
const int carYellow = 9;
const int buzzer = 10;
const int humanGreen = 11;
const int humanRed = 12;

bool busy = false;
int trafficState = 0;

const int buzzDuration = 100;
const int buzzToneLow = 5;
const int buzzToneHigh = 10;
const int buzzToneReg = 20;

//traffic states
const int carsGoing = 0;
const int carsStopping = 1;
const int delayBeforePedestrians = 2;
const int pedestriansStarting = 3;
const int pedestriansGoing = 4;
const int pedestriansStopping = 5;
const int delayBeforeCars = 6;
const int carsStarting = 7;

//read button variables
bool reading = LOW;
bool formerReading = LOW;
unsigned long long lastDebounceTimer = 0;
const int debounceInterval = 25;

//button click variables
bool formerButtonState = LOW;
bool buttonState = LOW;

//stop traffic variables
unsigned long long trafficStopTimestamp = 0;
bool trafficStopping = false;
const int yellowLightLength = 5000;
bool waitBeforeYellow = false;
const int waitBeforeYellowLength = 10000;

//process delay variables
bool waiting = false;
unsigned long long delayTimestamp = 0;
const int betweenLightsDelay = 2000;

//go pedestrians variables
bool walking = false;
unsigned long long goingTimestamp = 0;
const int pedestriansGoingInterval = 10000;
unsigned long long tickTimestamp = 0;
int tickLength = 1000;

//stop pedestrians variables
bool lit = true;
const int intermitLength = 500;
const int blinksCount = 10;
unsigned long long switchTimestamp = 0;
unsigned long long pedestrianStopTimestamp = 0;
const int pedestrianStopInterval = 3000;
bool stoppingPedestrian = false;


//methods
void readButtonState(bool& buttonState);
bool buttonClick();
void stopPedestrians();
void startPedestrians();
void stopTraffic();
void processDelay();
void processCurrentState();
void startTraffic();
void goPedestrians();

void setup() {
  // put your setup code here, to run once:
  pinMode(btn, INPUT_PULLUP);
  pinMode(carRed, OUTPUT);
  pinMode(carGreen, OUTPUT);
  pinMode(carYellow, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(humanGreen, OUTPUT);
  pinMode(humanRed, OUTPUT);

  digitalWrite(carGreen, HIGH);
  digitalWrite(humanRed, HIGH);
}

void loop() {
  if (!busy && buttonClick())
    trafficState = carsStopping;
  processCurrentState();
}

void readButtonState(bool& buttonState) {
  reading = !digitalRead(btn);
  if(reading != formerReading)
    lastDebounceTimer = millis();

  if(millis() - lastDebounceTimer > debounceInterval && reading != buttonState)
      buttonState = reading;
      
  formerReading = reading;
}


bool buttonClick() {
  bool pressed = false;
  readButtonState(buttonState);
  if(buttonState != formerButtonState) {
    formerButtonState = buttonState;
    if(buttonState)
      pressed = true;
  }
  return pressed;
}

void processCurrentState() {
  switch(trafficState) {
    case carsGoing:               break;
    case carsStopping:            busy = true; stopTraffic(); break;
    case delayBeforePedestrians:  processDelay(); break;
    case pedestriansStarting:     startPedestrians(); break;
    case pedestriansGoing:        goPedestrians(); break;
    case pedestriansStopping:     stopPedestrians(); break;
    case delayBeforeCars:         processDelay(); break;
    case carsStarting:            startTraffic(); busy = false; break;
  }
}

void stopTraffic() {
  if(!trafficStopping) {
    trafficStopTimestamp = millis();
    trafficStopping = true;
    waitBeforeYellow = true;
  }
  if(waitBeforeYellow && millis() - trafficStopTimestamp > waitBeforeYellowLength) {
    digitalWrite(carGreen, LOW);
    digitalWrite(carYellow, HIGH);
    trafficStopTimestamp = millis();
    waitBeforeYellow = false;
  }
  if(!waitBeforeYellow && millis() - trafficStopTimestamp > yellowLightLength) {
    digitalWrite(carYellow, LOW);
    digitalWrite(carRed, HIGH);
    trafficState = delayBeforePedestrians;
    trafficStopping = false;
  }
}

//method for the wait time before turning green
void processDelay() {
  if(!waiting) {
    waiting = true;
    delayTimestamp = millis();
  }
  if(millis() - delayTimestamp > betweenLightsDelay) {
    waiting = false;
    if(trafficState == delayBeforePedestrians) {
      trafficState = pedestriansStarting;
    }
    if(trafficState == delayBeforeCars) {
      trafficState = carsStarting;
    }
  }
}

void startPedestrians() {
  digitalWrite(humanGreen, HIGH);
  digitalWrite(humanRed, LOW);
  trafficState = pedestriansGoing;
}

void goPedestrians(){
  if(!walking) {
    goingTimestamp = millis();
    walking = true;
    tickTimestamp = millis();
    tone(buzzer, buzzToneReg, buzzDuration);
  }
  
  if(millis() - tickTimestamp > tickLength){
    tone(buzzer, buzzToneReg, buzzDuration);
    tickTimestamp = millis();
  }
  if(millis() - goingTimestamp > pedestriansGoingInterval){
    walking = false;
    trafficState = pedestriansStopping;
  }
}

void stopPedestrians() {
  if(!stoppingPedestrian) {
    pedestrianStopTimestamp = millis();
    stoppingPedestrian = true;
  }
  if(millis() - pedestrianStopTimestamp > pedestrianStopInterval) {
    digitalWrite(humanRed, HIGH);
    digitalWrite(humanGreen, LOW);
    trafficState = delayBeforeCars;
    lit = true;
    stoppingPedestrian = false;
  }
  if(millis() - switchTimestamp > intermitLength) {
    switchTimestamp = millis();
    lit = !lit;
    if(lit)
      tone(buzzer, buzzToneLow, buzzDuration);
     else
      tone(buzzer, buzzToneHigh, buzzDuration);
    digitalWrite(humanGreen, lit);
  }
}

void startTraffic() {
  digitalWrite(carRed, LOW);
  digitalWrite(carGreen, HIGH);
  trafficState = carsGoing;
}
