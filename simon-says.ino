int simonLeds[100];

int randomLed;
int leds[4] = { 2, 3, 4, 5 };
int buttons[4] = { 6, 7, 8, 9 };
int tones[4] = { 100, 101, 102, 103 };

int currentScore = 0;
int highscore = 0;

int roundDelay = 1000;
int sequenceDelay = 250;

int currentInput = -1;
int previousInput = -1;
int buttonsInput[4] = { 0, 0, 0, 0 };
int noInputCounter = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

void loop() {
  // wait until no buttons are being pressed
  waitForNoInput();

  // select random led and add selection to array
  simonSelect();

  // loop through led and tone sequence
  loopSimonSelections();

  // wait for user input
  // either record entire sequence and check at the end OR check at every input
  for (int i = 0; i <= currentScore; i++) {
    while (currentInput == -1 || currentInput == previousInput) {
      // loop through all buttons and check if they are pressed
      for (int j = 0; j < 4; j++) {
        buttonsInput[j] = digitalRead(buttons[j]);
        if (buttonsInput[j] == 0) {
          currentInput = j;
        } else {
          noInputCounter++;
        }
      }

      // if no buttons are pressed reset input variables
      if (noInputCounter == 4) {
        currentInput = -1;
        previousInput = -1;
      }
      noInputCounter = 0;
    }

    // verify user input
    if (currentInput != simonLeds[i]) {
      Serial.println("Wrong");
      lost();
      break;
    }
    // delay(250);
    previousInput = currentInput;
  }
  roundWin();

  // determine if entire input was correct

  Serial.println();
  delay(roundDelay);
}

void waitForNoInput() {
  bool noInput = true;
  while (!noInput) {
    noInput = true;
    for (int j = 0; j < 4; j++) {
      buttonsInput[j] = digitalRead(buttons[j]);
      if (buttonsInput[j] == 0) {
        noInput = false;
      }
    }
  }
}

void simonSelect() {
  randomLed = random(4);
  simonLeds[currentScore] = randomLed;
  for (int i = 0; i <= currentScore; i++) {
    Serial.print(simonLeds[i]);
    Serial.print(", ");
    Serial.println();
  }
}

void loopSimonSelections() {
  for (int i = 0; i <= currentScore; i++) {
    digitalWrite(leds[simonLeds[i]], HIGH);
    delay(sequenceDelay);
    digitalWrite(leds[simonLeds[i]], LOW);
    delay(sequenceDelay);
  }
}

void lost() {
  if (currentScore > highscore) {
    highscore = currentScore;
  }
  currentScore = 0;
}

void roundWin() {
  currentScore++;
}
