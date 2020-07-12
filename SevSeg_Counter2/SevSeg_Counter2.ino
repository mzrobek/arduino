/* SevSeg Counter Example
 
 Copyright 2017 Dean Reading
 Extended in 2020 by Maciej Zrobek
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 
 This example demonstrates a very simple use of the SevSeg library with a 4
 digit display. It displays a counter that counts up, showing elapsed time
 in the m.ss.d format (d is for deciseconds).
 The maximum elapsed time is 10 minutes.
 */

#define MINUTE 60
 
#include <stdio.h>
#include "SevSeg.h"
SevSeg sevseg; //Instantiate a seven segment controller object

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {5, 4, 3, 2};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = true; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void loop() {
  static unsigned long timer = millis();
  static int deciSeconds = 0;
  int seconds = 0;
  int minutes = 0;
  int tenthsOfSec = 0;
  char displayString[7];
  static bool overflown = false;
  
  if (!overflown && millis() - timer >= 100) {
    timer += 100;
    deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond
    
    if (deciSeconds == 10 * MINUTE * 10) { 
      // Display dashes when 10 minutes are reached
      overflown = true;
      strcpy(displayString, "----");
    }
    else
    {
      // Convert time in decisecs to mins, secs and decisecs
      minutes = deciSeconds / (10 * MINUTE);
      seconds = (deciSeconds - 10 * MINUTE * minutes) / 10;
      tenthsOfSec = deciSeconds - 10 * MINUTE * minutes - 10 * seconds;
      
      sprintf(displayString, "%d.%02d.%d", minutes, seconds, tenthsOfSec);
    }
       
    sevseg.setChars(displayString);
  }

  sevseg.refreshDisplay(); // Must run repeatedly
}

/// END ///
