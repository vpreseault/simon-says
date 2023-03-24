int simonLeds[100];

int randomLed;
int leds[4] = {9, 10, 11, 12};
int buttons[4] = {1,2,3,4};
int tones[4] = {100, 101, 102, 103}

int currentScore = 0;

void setup() {
  Serial.begin(9600);


  

}

void loop() {
  // select random led and add selection to array
  
  // loop through led and tone sequence

  // wait for user input 
  // either record entire sequence and check at the end OR check at every input

  // determine if entire input was correct

  // increment score



  randomLed = random(4);
  Serial.println(leds[randomLed]);
  delay(1000);
}
