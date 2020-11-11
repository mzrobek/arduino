/* A simple stopwatch using a 7-segment, 4-digit LED display

  Written on 09.11.2020 by Maciej Zrobek
  LED interface based on example by 2017 (C) Dean Reading

  The stopwatch can measurre time up to 10 minutes and displays the running time in m.ss.d format (d is for deciseconds)
  Initially the stopwatch displays 0.00.0
  Pressing the switch starts/pauses the stopwatch
  After 10 minutes are reached the stopwatch displays ----

*/

#define MINUTE 60
// Timer machine states
#define ST_READY      0   // Stopwatch not yet started
#define ST_RUNNING    1   // Stopwatch started and running
#define ST_PAUSED     2   // Stopwatch paused
#define ST_OVERFLOW   3   // Stopwatch time has exceeded 10 minutes

#define SWITCH_PIN    A0  // Pin to which the tact switch is connected

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

  pinMode(SWITCH_PIN, INPUT);
  
  // LED initialisation
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void loop() {
  
  static unsigned long timer = millis();  // Current time from starting Arduino (not the stopwatch)
  static int deciSeconds = 0;
  static byte pgmState = ST_READY;
  static byte prevPgmState = ST_READY;
  int seconds = 0;
  int minutes = 0;
  int tenthsOfSec = 0;
  char displayString[7]; 
  byte switchState;
  static byte prevSwitchState = LOW;

  switchState = digitalRead(SWITCH_PIN);

  switch (pgmState)
  {
    case ST_READY:
      // We start the operation with displaying 0.00.0
      strcpy(displayString, STR_ZEROES);
      if (switchState == HIGH && prevSwitchState == LOW) // Double check to deal with long switch press
      {
        if (pgmState == prevPgmState)
        {
          pgmState = ST_RUNNING; // Start the stopwatch
        }
        prevPgmState = pgmState;
      }
      break;

    case ST_RUNNING:
      if (millis() - timer >= 100)
      {
        // Noemal stopwatch operation
        timer += 100;
        deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond

        if (deciSeconds == 10 * MINUTE * 10)
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
      
      if (switchState == HIGH && prevSwitchState == LOW)
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
      if (switchState == HIGH && prevSwitchState == LOW)
      {
        if (pgmState == prevPgmState)
        {
          pgmState = ST_RUNNING; // Unpause the stopwatch
        }
        prevPgmState = pgmState;
      }

      break;
      
    case ST_OVERFLOW:
      strcpy(displayString, STR_OVERFLOW);
      break;
  }

  sevseg.setChars(displayString);
  sevseg.refreshDisplay(); // Must run repeatedly

  prevSwitchState = switchState;
  delay (5); // To avoid flickering
}

/// END ///
