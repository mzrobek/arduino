//------- ELEMENTS    ---------
//-- 1 x potentiometer 10 kB
//-- 1 x passive buzzer
//------- CONNECTIONS ---------
//-- Left pin of pot -> 5V
//-- Middle pin of pot -> A0
//-- Right pin of pot -> GND
//-- Buzzer pin 1 -> GND
//-- Buzzer pin 2 -> Digital Output 9

const int potPin = A0;
const int buzzerPin = 8;
const int lowFreq = 500;
const int hiFreq = 10000;

int   potInput = 0;
int   pitch = 0;

void setup() {
  // put your setup code here, to run once: 

}

void loop() {
  // put your main code here, to run repeatedly:
  potInput  = analogRead(potInput);
  pitch = map(potInput, 0, 1023, lowFreq, hiFreq);
  tone(buzzerPin, pitch, 20);
  delay(10);
}
