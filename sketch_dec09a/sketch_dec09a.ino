int WEIGHT_OUT =  2;
int WEIGHT_SENSE = 3; 
int POMODORRO_OUT = 12;
int POMODORRO_BUTTON = 11;

int PIEZO = 4;

int SittingLED = 13;
int PomLED = 10;

double TWENTY_FIVE_MINS = 3000; // 500MS BLOCKS
double FIVE_MINUTES = 600; 


bool pomodorroStatus = false;
bool sittingStatus = false;
bool working = true; // default is staart working when entering program

double count = 0;

void flashingPom(){
  int ii =0; 
  while(ii <= 3){
    digitalWrite(PomLED, HIGH);
    delay(100);
    digitalWrite(PomLED, LOW);
    delay(100);
    ii++;
  }
}

void turnPomodorroOn(){
  int pomSense = digitalRead(POMODORRO_BUTTON);
  if (pomSense == LOW) {
    delay(200); // debounce 
    if (pomodorroStatus == false) {
      flashingPom();
      pomodorroStatus = true;
      digitalWrite(PomLED, LOW);
      working = true;
    } else {
      // reset to defaults
      pomodorroStatus = false;
      digitalWrite(PomLED, HIGH);
      digitalWrite(SittingLED, LOW);
      count = 0;
      working = true;
      sittingStatus = false;
    }
  }
}

void checkSittinSensor(){
  int senseResults = digitalRead(WEIGHT_SENSE);
  if (pomodorroStatus == true){
    if (senseResults == LOW) {
      sittingStatus = true;
    }
    else {
      sittingStatus = false;
    }
  } else { // turn on pomodorro automatically if sitting 
    if (senseResults == LOW) {
      pomodorroStatus = true;
      sittingStatus = true;
    }
  }
}

void pomTimer(){
  if (pomodorroStatus == true) { // only added in to make code stringent
    count++;
    // toggle if exceeded time
    if (working == false){
      digitalWrite(SittingLED, LOW);
      if (count >= FIVE_MINUTES) {
        working = true;
        count = 0;
      }
      // if sitting while not working PIZO beep!
      if (sittingStatus == true){
        tone(PIEZO, 500);
        return;
      }  
    }
    else {
      digitalWrite(SittingLED, HIGH);
      if (count >= TWENTY_FIVE_MINS) {
        working = false;
        count = 0;
      }
      // if not sitting during working PIEZO beep!
      if (sittingStatus == false){
        tone(PIEZO, 500);
        return;
      }
    }
  }
  noTone(PIEZO);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(WEIGHT_OUT, OUTPUT);
  pinMode(WEIGHT_SENSE, INPUT);
  pinMode(POMODORRO_BUTTON, INPUT);
  pinMode(POMODORRO_OUT, OUTPUT);
  pinMode(PomLED, OUTPUT);
  pinMode(SittingLED, OUTPUT);
  pinMode(PIEZO, OUTPUT);
}

void loop() {
  
  digitalWrite(WEIGHT_OUT, HIGH); // turn on 5V check weight sensor
  digitalWrite(POMODORRO_OUT, HIGH); // turn on 5v button sensor
  if (pomodorroStatus == false) digitalWrite(PomLED, HIGH); // turn on by default
  else digitalWrite(PomLED, LOW);

  turnPomodorroOn();
  checkSittinSensor();
  pomTimer();
  delay(500); 
  
}



