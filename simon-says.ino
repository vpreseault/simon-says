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
  userInput();

  delay(roundDelay);
}

void waitForNoInput() {
  Serial.println("waitForNoInput \n");
  bool noInput;
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
  Serial.println("simonSelect \n");
  randomLed = random(4);
  simonLeds[currentScore] = randomLed;  
}

void loopSimonSelections() {
  Serial.println("loopSimonSelections");
  for (int i = 0; i <= currentScore; i++) {
    Serial.print(simonLeds[i]);
    Serial.print(", ");

    digitalWrite(leds[simonLeds[i]], HIGH);
    delay(sequenceDelay);
    digitalWrite(leds[simonLeds[i]], LOW);
    delay(sequenceDelay);
  }
  Serial.println("\n");
}

void userInput() {
  Serial.println("userInput \n");
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
      lost();
      return;
    }
    previousInput = currentInput;
  }
  roundWin();
}

void lost() {
  Serial.println("lost \n");
  if (currentScore > highscore) {
    highscore = currentScore;
  }
  currentScore = 0;
  currentInput = -1;
  previousInput = -1;
}

void roundWin() {
  Serial.println("roundWin \n");
  currentScore++;
}
