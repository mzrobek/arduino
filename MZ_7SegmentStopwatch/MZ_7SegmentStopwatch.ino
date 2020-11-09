/* A simple stopwatch using a 7-segment, 4-digit LED display
 
 Written on 09.11.2020 by Maciej Zrobek
 Based on example by 2017 (C) Dean Reading
 
 The stopwatch can measurre time up to 10 minutes and displays the running time in m.ss.d format (d is for deciseconds)
 Initially the stopwatch displays 0.00.0
 Pressing the switch starts/stops the stopwatch
 After 10 minutes are reached the stopwatch displays -.--.-
 
 */

#define MINUTE 60

#define ST_READY      0
#define ST_RUNNING    1
#define ST_STOPPED    2
#define ST_OVERFLOW   3
#define SWITCH_PIN    A0

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
  Serial.begin(9600);
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void loop() {
  static unsigned long timer = millis();
  static int deciSeconds = 0;
  static byte pgmState = ST_READY;
  static byte prevPgmState = ST_READY;
  int seconds = 0;
  int minutes = 0;
  int tenthsOfSec = 0;
  char displayString[7];
  byte switchState;
  static byte prevSwitchState = LOW;
  static bool overflown = false;

  //Serial.print(switchState);
  //Serial.println();
  
  switchState = digitalRead(SWITCH_PIN);
    
  switch (pgmState)
  {
    case ST_READY: 
      strcpy(displayString, STR_ZEROES);
      if (switchState == HIGH && prevSwitchState == LOW)
      {
        if (pgmState == prevPgmState)
        {
          pgmState = ST_RUNNING;
        }  
        prevPgmState = pgmState;
      }
      break;
      
    case ST_RUNNING:
      strcpy(displayString, "AAAA");
      if (switchState == HIGH && prevSwitchState == LOW)
      {
        if (pgmState == prevPgmState)
        {
          pgmState = ST_STOPPED;
        }
        prevPgmState = pgmState;  
      }
      break;
      
    case ST_STOPPED:
      strcpy(displayString, "CCCC");
      if (switchState == HIGH && prevSwitchState == LOW)
      {
        if (pgmState == prevPgmState)
        {
          pgmState = ST_RUNNING;
        } 
        prevPgmState = pgmState; 
      }
  
      break;  
    case ST_OVERFLOW:
      strcpy(displayString, STR_OVERFLOW);  
      break;
  }
  
//  if (!overflown && millis() - timer >= 100) {
//    timer += 100;
//    deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond
//    
//    if (deciSeconds == 10 * MINUTE * 10) { 
//      // Display dashes when 10 minutes are reached
//      overflown = true;
//      strcpy(displayString, "----");
//    }
//    else
//    {
//      // Convert time in decisecs to mins, secs and decisecs
//      minutes = deciSeconds / (10 * MINUTE);
//      seconds = (deciSeconds - 10 * MINUTE * minutes) / 10;
//      tenthsOfSec = deciSeconds - 10 * MINUTE * minutes - 10 * seconds;
//      
//      sprintf(displayString, "%d.%02d.%d", minutes, seconds, tenthsOfSec);
//    }
//       
//    sevseg.setChars(displayString);
//  }


 
  
//  Serial.print("state="); Serial.print(pgmState); 
//  Serial.print(", prevstate="); Serial.print(prevPgmState); 
//  Serial.print(", switch="); Serial.print(switchState); 
//  Serial.print(", prevswitch="); Serial.print(prevSwitchState); 
//  Serial.println();
  sevseg.setChars(displayString);
  sevseg.refreshDisplay(); // Must run repeatedly
  prevSwitchState = switchState;
//  delay(500);
}

/// END ///
