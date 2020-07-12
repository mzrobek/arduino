
#include "SevSeg.h"
SevSeg sevseg;

void setup() {

  byte numDigits = 1;     // Number of digits
  byte digitPins[] = {};  // Must be empty for single disgit displays
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
  sevseg.setBrightness(10);
}

void loop() {
  for (int i = 0; i < 10; i++) {
    sevseg.setNumber(i, i % 2);
    delay(500);
    sevseg.refreshDisplay();
  }
}
