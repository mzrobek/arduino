#include "LedControl.h"

#define PIN_DIN 2
#define PIN_CS 3
#define PIN_CLK 4
#define NUM_DEVICES 1
#define DELAY_TIME 2000 

LedControl lc = LedControl(PIN_DIN, PIN_CLK, PIN_CS, NUM_DEVICES);

const byte Grid[] = {
  0B10101010,
  0B01010101,
  0B10101010,
  0B01010101,
  0B10101010,
  0B01010101,
  0B10101010,
  0B01010101
};

const byte char_J[] =           {0x00,0x3c,0x04,0x04,0x04,0x04,0x24,0x18};
const byte char_E[] =           {0x00,0x3c,0x20,0x20,0x38,0x20,0x20,0x3c};
const byte char_B[] =           {0x00,0x38,0x24,0x24,0x38,0x24,0x24,0x38};
const byte char_A[] =           {0x00,0x18,0x24,0x24,0x3c,0x24,0x24,0x24};
const byte char_c_accented[] =  {0x04,0x08,0x18,0x24,0x20,0x20,0x24,0x18};
const byte char_P[] =           {0x00,0x38,0x24,0x24,0x38,0x20,0x20,0x20};
const byte char_i[] =           {0x00,0x00,0x08,0x00,0x08,0x08,0x08,0x08};
const byte char_S[] =           {0x00,0x1c,0x20,0x20,0x18,0x04,0x04,0x38};

const byte* theMessage[] = {char_J, char_E, char_B, char_A, char_c_accented, char_P, char_i, char_S, NULL};


void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0, false);
  lc.setIntensity(0,2);
  lc.clearDisplay(0);
}

void displayMatrix(int addr, const byte rows[], unsigned int delaytime = 0)
{
  for (int row = 0; row < 8; row++)
  { 
    lc.setRow(addr, row, rows[row]);  
    delay(delaytime);
  }
}

void displayString(int addr, const byte* text[], unsigned int delaytime)
{
  const byte** ptr = text;    
  while (*ptr != NULL)
  { 
    displayMatrix(addr, *ptr);
    delay(delaytime);
    lc.clearDisplay(addr);
    ptr++;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  displayString(0, theMessage, 500);
}
