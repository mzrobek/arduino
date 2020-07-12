// Uncomment for Serial Monitor output
//#define SERIAL_OUTPUT

//------- ELEMENTS    ---------
//-- 1 x potentiometer 10 kB
//-- 1 x resistor 220 R
//-- 1 x capacitor 100 uF
//------- CONNECTIONS ---------
//-- Left pin of pot -> 5V
//-- Middle pin of pot -> A0
//-- Right pin of pot -> GND
//-- LED anode -> GND
//-- LED cathode -> 220 R resistor pin 1
//-- 220 R resistor pin 2 -> Digital Output 9
//-- 100 uF capacitor - parallel to power supply
// Works better when Arduino is powered from an external source
// and when the pot is plugged into the breadboard directly
// rather than from USB (less flicker)

const int potPin = A0;
const int ledPin = 9;

int   potInput = 0;
int   ledOutput = 0;

void setup() {
  // put your setup code here, to run once:
#ifdef SERIAL_OUTPUT  
  Serial.begin(9600);
#endif  
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  potInput  = analogRead(potInput);
#ifdef SERIAL_OUTPUT  
  // Output to serial causes the LED to flicker
  Serial.print("Potentiometer input = ");
  Serial.println(potInput);
#endif  
  ledOutput = potInput / 4;
  analogWrite(ledPin, ledOutput);
#ifdef SERIAL_OUTPUT  
  delay(100);
#endif  
}
