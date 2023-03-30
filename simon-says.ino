#include <LiquidCrystal.h>
const int rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int simonLeds[100];

int randomLed;
int leds[4] = { 2, 3, 4, 5 };
int buttons[4] = { 17, 16, 15, 14 };
int tones[4] = { 440, 523, 659, 784 };
int buzzerPin = 12;

int currentScore = 0;
int highscore = 0;

int roundDelay = 1000;
int sequenceDelay = 250;

int buttonsInput[4] = { 0, 0, 0, 0 };
int noInputCounter = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(buzzerPin, OUTPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

void loop() {
  // reset LCD
  updateLCDScores();

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

void userInput() {
  Serial.println("userInput \n");
  for (int i = 0; i <= currentScore; i++) {
    while (arraySum(buttonsInput, 4) == 4) {
      // loop through all buttons and check if they are pressed
      for (int j = 0; j < 4; j++) {
        buttonsInput[j] = digitalRead(buttons[j]);
        if (buttonsInput[j] == 0) {
          digitalWrite(leds[j], HIGH);
          tone(buzzerPin, tones[j]);

          // verify user input if button[j] is pressed
          if (j != simonLeds[i]) {
            waitForNoInput();
            resetOutputs();
            lost();
            return;
          }
        }
      }
    }
    waitForNoInput();
    resetOutputs();
  }
  roundWin();
}

void waitForNoInput() {
  Serial.println("waitForNoInput \n");
  while (arraySum(buttonsInput, 4) != 4) {
    for (int j = 0; j < 4; j++) {
      buttonsInput[j] = digitalRead(buttons[j]);
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
    tone(buzzerPin, tones[simonLeds[i]]);
    delay(sequenceDelay);
    digitalWrite(leds[simonLeds[i]], LOW);
    noTone(buzzerPin);
    delay(sequenceDelay);
  }
  Serial.println("\n");
}

void lost() {
  Serial.println("lost \n");
  currentScore = 0;
  
  losingAnimation();
}

void losingAnimation() {
  Serial.println("losingAnimation \n");
  delay(sequenceDelay);
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
    tone(buzzerPin, tones[i]);
    delay(50);
    digitalWrite(leds[i], LOW);
    noTone(buzzerPin);
    delay(50);
  }
}

void roundWin() {
  Serial.println("roundWin \n");
  currentScore++;

  if (currentScore > highscore) {
    highscore = currentScore;
  }

  updateLCDScores();
}

void updateLCDScores() {
  Serial.println("updateLCDScores \n");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: " + String(currentScore));
  lcd.setCursor(0, 1);
  lcd.print("Best: " + String(highscore));
}

void resetOutputs() {
  Serial.println("resetOutputs \n");
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], LOW);
  }
  noTone(buzzerPin);
}

int arraySum(int array[], int length) {
  int sum = 0;
  for (int i = 0; i < length; i++) {
    sum += array[i];
  }
  return sum;
}
