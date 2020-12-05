/* The Rear Bike Light Simulator

  Written on 05.12.2020 by Maciej Zrobek

  This project was inspired by the rear LED light in my bicycle
  It has several light patterns which are cycled through
  Initially all LEDs are off - press the switch to start the first pattern
  Short press of the switch moves to the next pattern
  Long press of the switch turns all LEDs off and resets the cycle 

*/

const byte numLeds              = 3;          // Number of LEDs should be odd
const byte ledPins[numLeds]     = {2, 3, 4};  // Pins to which the LEDs are connected via resistors
const byte switchPin            = 12;         // Pin to which the tact switch is connected

byte currentIndex  = 0;

void switchLedsOff()
{
  // The purpose of this function is evident
  for (byte i = 0; i < numLeds; i++)
  {
    byte pin = ledPins[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  currentIndex = 0;
}

void setup() {
  switchLedsOff();
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
    // Turn the device on
    switchPressTime = loopTime = millis();
    active = true;
  }
  else if (switchState == HIGH && prevSwitchState == HIGH)
  {
    unsigned long switchPressDuration = millis() - switchPressTime; // Check for how long the switch has been depressed
    if (switchPressDuration > 2000)
    {
      // Depressed for long enough to turn the device off
      switchLedsOff();
      active = false;
    }
  }

  prevSwitchState = switchState;

  if (!active)
    return; // Just keep all LEDs dark

  byte pin = ledPins[currentIndex];
  unsigned long now = millis();
  Serial.print("now = "); 
  Serial.print(now);
  Serial.print(", loopTime = "); 
  Serial.println(loopTime);

  // This is the implementation of a sample "moving light"
  // pattern for testing purposes
  // We can't use the delay() function because we want to be able to
  // React to the switch at any moment 
  if (now - loopTime <= 500)
  {
    // No time to change the active LED yet
    digitalWrite(pin, HIGH);
//    Serial.print("Switching on LED ");
//    Serial.println(currentIndex);
  }
  else
  {
    digitalWrite(pin, LOW);
//    Serial.print("Switching off LED ");
//    Serial.println(currentIndex);
    // Time to turn the current LED off and move to the next one
    if (currentIndex++ == numLeds)
      currentIndex = 0; // Last LED turned off - wrap around 
    loopTime = now;  
  }

}
