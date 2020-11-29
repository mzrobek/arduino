#define NUM_LEDS                3
const byte numLeds              = 3;
const byte ledPins[numLeds]     = {2, 3, 4};
const byte switchPin            = 12;

byte currentIndex  = 0;

void switchLedsOff()
{
  for (byte i = 0; i < numLeds; i++)
  {
    byte pin = ledPins[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  currentIndex = 0;
}

void setup() {
  // put your setup code here, to run once:
  switchLedsOff();
  pinMode(switchPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  static bool active = false;
  static byte prevSwitchState = LOW;
  static unsigned long switchPressTime = 0UL;
  static unsigned long loopTime = 0UL;

  // put your main code here, to run repeatedly:
  int switchState = digitalRead(switchPin);
  //  Serial.print("Switch state=");
  //  Serial.println(switchState ? "HIGH" : "LOW");

  if (switchState == HIGH && prevSwitchState == LOW)
  {
    switchPressTime = loopTime = millis();
    active = true;
  }
  else if (switchState == HIGH && prevSwitchState == HIGH)
  {
    unsigned long switchPressDuration = millis() - switchPressTime;
    if (switchPressDuration > 2000)
    {
      switchLedsOff();
      active = false;
    }
  }

  prevSwitchState = switchState;

  if (!active)
    return;

  byte pin = ledPins[currentIndex];
  unsigned long now = millis();
  Serial.print("now = "); 
  Serial.print(now);
  Serial.print(", loopTime = "); 
  Serial.println(loopTime);
  
  if (now - loopTime <= 500)
  {
    digitalWrite(pin, HIGH);
//    Serial.print("Switching on LED ");
//    Serial.println(currentIndex);
  }
  else
  {
    digitalWrite(pin, LOW);
//    Serial.print("Switching off LED ");
//    Serial.println(currentIndex);
    if (currentIndex++ == NUM_LEDS)
      currentIndex = 0;
      loopTime = now;  
  }

//  for (byte i = 0; i < numLeds; i++)
//  {
//    byte pin = ledPins[i];
//    digitalWrite(pin, HIGH);
//    delay(50);
//    digitalWrite(pin, LOW);
//    delay(50);
//  }

}
