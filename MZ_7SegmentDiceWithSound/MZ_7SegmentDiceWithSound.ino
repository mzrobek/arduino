
/***************************************************************
 *********** The Annoying 7-Segment LED Dice *******************
 ***************************************************************
 *  Written by Maciej Zrobek, 29.12.2019
 * 
 *  Press and release the microswitch to generate random numbers
 *  and display them on a 7-segment LED with accompanying noise
 *  from the buzzer
 *  
 *  Components:
 *  1 x 7-segment, single digit LED display
 *  1 x tact switch
 *  1 x 220 Ohm resistor
 *  1 x 1 kOhm resistor
 ****************************************************************/

#include "SevSeg.h"
SevSeg sevseg;

const int switchPin = 10;
const int buzzerPin = 11;
const int displayDelay = 50;
const unsigned long blinkDelay = 200;
const int maxScore = 6;
const int minFreq = 1000;
const int maxFreq = 2000;
int switchState = LOW;
int blinkState = HIGH;
unsigned long previousMillis = 0;

void setup()
{

  byte numDigits = 1;     // Number of digits
  byte digitPins[] = {};  // Must be empty for single digit displays
  // Pins connected to segments:
  //    AAAAA
  //   F     B
  //   F     B
  //    GGGGG
  //   E     C
  //   E     C
  //    DDDDD   DP
  //                    A  B  C  D  E  F  G  DP
  byte segmentPins[] = {4, 5, 7, 8, 9, 3, 2, 6};
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
  sevseg.setNumber(0); // Initially display a zero
  delay(displayDelay); // Somehow it doesn't work without a delay
  sevseg.refreshDisplay();
  pinMode(switchPin, INPUT);
  pinMode(switchPin, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0)); // Assume pin 0 is unconnected and its state is random
}

void makeNoise()
{
  int freq = random(minFreq, maxFreq + 1);
  tone(buzzerPin, freq);
}

void loop()
{
  unsigned long currentMillis = 0;
  int freq = 0;

  switchState = digitalRead(switchPin);
  if (switchState == HIGH)
  {
    // As long as the switch is pressed blink a zero
    while ((switchState = digitalRead(switchPin)) == HIGH)
    {
      // I am not using delay() because I want the score to be displayed immediately
      // after releasing the switch
      currentMillis = millis();
      if (currentMillis - previousMillis >= blinkDelay) // This will display the zero immediately in the 1st loop
      {
        if (blinkState == HIGH)
        {
          sevseg.setNumber(0);
          sevseg.refreshDisplay();
          makeNoise();
          blinkState = LOW;
        }
        else
        {
          sevseg.blank(); // blank() doesn't require refreshDisplay() to work
          freq = random(minFreq, maxFreq + 1);
          makeNoise();
          blinkState = HIGH;
        }
        previousMillis = currentMillis;
      }
    }
    int score = random(1, maxScore + 1); // Add 1 because random() generates a number SMALLER than the 2nd arg
    sevseg.setNumber(score); // Display a random score
    sevseg.refreshDisplay();
    noTone(buzzerPin);
  }

}
