#include <Servo.h>

// Stworzenie obiektu serwomechanizmu
Servo sg90;

void setup()
{
  // Ustawienie pinu do ktorego podlaczone jest serwo
  sg90.attach(2);
}

void loop()
{
  // ustawienie osi na minimalny kat
  sg90.writeMicroseconds(450);
  // oczekiwanie pol sekundy
  delay(500);
  
  // ustawieie osi na srodkowy kat
  sg90.writeMicroseconds(1450);
  delay(500);
  
  // ustawienie osi na maksymalny kat
  sg90.writeMicroseconds(2450);
  delay(500);
  
  sg90.writeMicroseconds(1450);
  delay(500);
}
