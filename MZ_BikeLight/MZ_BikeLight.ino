/* The Rear Bike Light Simulator

  Written on 20.12.2020 by Maciej Zrobek

  This project was inspired by the rear LED light in my bicycle
  It has several light sequences which are cycled through
  Each sequence consists of several LED patterns which are displayed for a fixed duration
  Initially all LEDs are off - press the switch to start the first sequence
  Short press of the switch moves to the next sequence
  Long press of the switch turns all LEDs off and resets the cycle

*/

const byte numLeds              = 3;          // Number of LEDs should be odd
const byte numSeqs              = 5;          // Number of light sequences
const byte maxPatterns          = 15;         // Maximum number of patterns in a sequence
const byte EOS                  = 0XFF;       // The marker of the end of sequence
const byte ledPins[numLeds]     = {2, 3, 4};  // Pins to which the LEDs are connected via resistors
const byte switchPin            = 12;         // Pin to which the tact switch is connected
const int  patternDelay         = 200;        // Delay between patterns in ms

// The table of light sequences
// A pattern in a sequence is coded with bits which correspond to LEDs being turned on or off
// (e.g. 0B101 means turn the LEDs 1 and 3 on)
// A sequence ends with the EOS marker
// The patterns in a sequence are processed every patternDelay millliseconds
const byte seqs[numSeqs][maxPatterns] = {
    {0B100, 0B010, 0B001, EOS},         // Moving light
    {0B111, 0B000, EOS},                // All blinking
    {0B101, 0B101, 0B000, 0B000, 0B101, 0B101, 0B000, 0B000, 0B010, 0B000, 0B010, 0B000, 0B010, 0B000, EOS}, // Fancy blink 1
    {0B101, 0B010, 0B101, 0B000, EOS},  // Fancy blink 2
    {0B111, EOS}                        // All on
//  {0B100,EOS}, {0B010, EOS}, {0B001, EOS} 
};

byte currSeq  = 0;
byte currPattern = 0;

void displayPattern(byte pattern)
{
  byte mask = 1 << (numLeds - 1);
  for (byte i = 0; i < numLeds; i++)
  {
    if (pattern & mask)
      digitalWrite(ledPins[i], HIGH);
    else
      digitalWrite(ledPins[i], LOW);
    mask = mask >> 1;
  }

}

void reset()
{
  displayPattern(0); // Turn all LEDs off
  currSeq = 0;
  currPattern = 0;
}

void setup() {
  for (byte i = 0; i < numLeds; i++)
    pinMode(ledPins[i], OUTPUT);
  reset();
  pinMode(switchPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  static bool active = false;                   // true when the device is on
  static byte prevSwitchState = LOW;
  static unsigned long switchPressTime = 0UL;   // The point in time when the switch was last pressed
  static unsigned long loopTime = 0UL;          // The point in time when the LED state was last updated

  int switchState = digitalRead(switchPin);
//  Serial.print("Switch state=");
//  Serial.println(switchState ? "HIGH" : "LOW");

  if (switchState == HIGH && prevSwitchState == LOW)
  {

    // This delay prevents immediate moving to the next seq
    delay(100); 
    if (!active)
    {
      active = true; // Turn the device on
      //Serial.println("*** BREAKPOINT 1 ***");
    }
    else
    {
      // Move on to the next sequence
      // Wrap around if it was the last sequence
      //Serial.println("*** BREAKPOINT 2 ***");
      if (++currSeq == numSeqs)
        currSeq = 0;
      currPattern  = 0;
    }
    
    switchPressTime = loopTime = millis();
  }
  else if (switchState == HIGH && prevSwitchState == HIGH)
  {
    unsigned long switchPressDuration = millis() - switchPressTime; // Check for how long the switch has been depressed
    if (switchPressDuration > 2000)
    {
      //Serial.println("*** BREAKPOINT 3 ***");
      // Depressed for long enough to turn the device off
      reset();
      active = false;
    }
  }

  prevSwitchState = switchState;

  if (!active)
    return; // Just keep all LEDs dark

  byte pattern = seqs[currSeq][currPattern];
  unsigned long now = millis();
  //  Serial.print("now = ");
  //  Serial.print(now);
  //  Serial.print(", loopTime = ");
  //  Serial.println(loopTime);

  // We can't use the delay() function because we want to be able to
  // React to the switch at any moment
  if (now - loopTime > patternDelay)
  {
    // Time to update the pattern
    displayPattern(pattern);
    // Move on to the next pattern
    pattern = seqs[currSeq][++currPattern];
    if (pattern == EOS)
      currPattern = 0; // Last pattern in the sequence - wrap around
    loopTime = now;
  }

}
