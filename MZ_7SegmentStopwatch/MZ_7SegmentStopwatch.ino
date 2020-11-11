/* A simple stopwatch using a 7-segment, 4-digit LED display

  Written on 09.11.2020 by Maciej Zrobek
  LED interface based on example by 2017 (C) Dean Reading

  The stopwatch can measurre time up to 10 minutes and displays the running time in m.ss.d format (d is for deciseconds)
  Initially the stopwatch displays 0.00.0
  Pressing the 1st switch starts/pauses the stopwatch
  After 10 minutes reached the stopwatch displays ---- (overflow)
  While the stopwatch is paused or oveflown pressing the 2nd switch resets the stopwatch to zero

*/

#define MINUTE            60
#define OVERFLOW_MINUTES  10
// Timer machine states
#define ST_READY      0   // Stopwatch not yet started
#define ST_RUNNING    1   // Stopwatch started and running
#define ST_PAUSED     2   // Stopwatch paused
#define ST_OVERFLOW   3   // Stopwatch time has exceeded 10 minutes

#define SWITCH1_PIN    A0  // Start/Stop switch
#define SWITCH2_PIN    A1  // Reset switch

#define STR_ZEROES    "0.00.0"
#define STR_OVERFLOW  "----"

#include <stdio.h>
#include "SevSeg.h"
SevSeg sevseg; //Instantiate a seven segment controller object

void setup() {

  byte hardwareConfig = COMMON_CATHODE; // Using common cathode display
  byte numDigits = 4;
  //                  D1 D2 D3 D4
  byte digitPins[] = {5, 4, 3, 2}; // Pins which are connected to segment cathodes
  //                    A  B  C  D   E  F   G   DP
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // Pins which are connected to segments via resitors
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins

  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = true; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected

  pinMode(SWITCH1_PIN, INPUT);
  pinMode(SWITCH2_PIN, INPUT);

  // LED initialisation
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void loop() {

  static unsigned long timer = 0UL;  // Time in ms elapsed from starting the stopwatch
  static int deciSeconds = 0;
  static byte pgmState = ST_READY;
  static byte prevPgmState = ST_READY;
  int seconds = 0;
  int minutes = 0;
  int tenthsOfSec = 0;
  char displayString[7];
  byte switchState1, switchState2;
  static byte prevSwitchState1 = LOW;
  static byte prevSwitchState2 = LOW;

  switchState1 = digitalRead(SWITCH1_PIN);
  switchState2 = digitalRead(SWITCH2_PIN);

  switch (pgmState)
  {
    case ST_READY:
      // We start the operation with displaying 0.00.0
      strcpy(displayString, STR_ZEROES);
      if (switchState1 == HIGH && prevSwitchState1 == LOW) // Double check to deal with long switch press
      {
        if (pgmState == prevPgmState)
        {
          timer = millis();
          pgmState = ST_RUNNING; // Start the stopwatch
        }
        prevPgmState = pgmState;
      }
      break;

    case ST_RUNNING:
      if (millis() - timer >= 100)
      {
        // Normal stopwatch operation
        timer += 100;
        deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond

        if (deciSeconds == OVERFLOW_MINUTES * MINUTE * 10)
        {
          // Display dashes when 10 minutes are reached
          pgmState = ST_OVERFLOW;
        }
        else
        {
          // Convert time in decisecs to mins, secs and decisecs
          minutes = deciSeconds / (10 * MINUTE);
          seconds = (deciSeconds - 10 * MINUTE * minutes) / 10;
          tenthsOfSec = deciSeconds - 10 * MINUTE * minutes - 10 * seconds;
          // Format the string to be displayed
          sprintf(displayString, "%d.%02d.%d", minutes, seconds, tenthsOfSec);
        }
      }

      if (switchState1 == HIGH && prevSwitchState1 == LOW)
      {
        if (pgmState == prevPgmState)
        {
          pgmState = ST_PAUSED; // Pause the stopwatch
        }
        prevPgmState = pgmState;
      }
      break;

    case ST_PAUSED:
      timer = millis(); // If we don't update the timer the stopwatch will be still running in the background
      if (switchState1 == HIGH && prevSwitchState1 == LOW)
      {
        if (pgmState == prevPgmState)
        {
          pgmState = ST_RUNNING; // Unpause the stopwatch
        }
        prevPgmState = pgmState;
        break;
      }

      if (switchState2 == HIGH && prevSwitchState2 == LOW)
      {
        if (pgmState == prevPgmState)
        {
          // Reset the stopwatch
          deciSeconds = 0;
          pgmState = ST_READY;
        }
        prevPgmState = pgmState;
        break;
      }

      break;

    case ST_OVERFLOW:
      if (switchState2 == HIGH && prevSwitchState2 == LOW)
      {
        // Reset the stopwatch
        deciSeconds = 0;
        pgmState = ST_READY;
        prevPgmState = pgmState;
        break;
      }
      strcpy(displayString, STR_OVERFLOW);
      break;
  }

  sevseg.setChars(displayString);
  sevseg.refreshDisplay(); // Must run repeatedly

  prevSwitchState1 = switchState1;
  prevSwitchState2 = switchState2;
  delay (5); // To avoid flickering
}

/// END ///
