/***************************************************************
 *********** LED Matrix Display Demo         *******************
 ***************************************************************
    Written by Maciej Zrobek, 14.03.2021

    Display a text on a MAX72xx based 8x8 LED matrix

    Components:
    A 8x8 LED matrix with a MAX72xx chip
 ****************************************************************/


#include "LedControl.h"

// MAX72xx pinouts
#define PIN_DIN                 2
#define PIN_CS                  3
#define PIN_CLK                 4
#define NUM_DEVICES             1
#define NUM_ROWS                8
#define NUM_COLS                8
#define DELAY_BETWEEN_CHARS 300 // The default delay after displaying the matrix in msecs
// Display effects - more to come in the future
#define DISP_MODE_APPEAR      0  // Just display
#define DISP_MODE_SLIDE_LEFT  1  // Slide in from right to left

LedControl lc = LedControl(PIN_DIN, PIN_CLK, PIN_CS, NUM_DEVICES);

// The string to be displayed must be coded as a NULL-terminated 2-dimensional array of bytes
// with each element being one character to be displayed which in turn is coded as an array of bytes (rows)

const byte theGrid[] = 
{  // A checkered flag to be used as a separator
  0B10101010,
  0B01010101,
  0B10101010,
  0B01010101,
  0B10101010,
  0B01010101,
  0B10101010,
  0B01010101
};

// Some characters
// Use https://www.riyas.org/2013/12/online-led-matrix-font-generator-with.html to code the byte values
// 
const byte char_J[] =           {0x00, 0x3c, 0x04, 0x04, 0x04, 0x04, 0x24, 0x18};
const byte char_E[] =           {0x00, 0x3c, 0x20, 0x20, 0x38, 0x20, 0x20, 0x3c};
const byte char_B[] =           {0x00, 0x38, 0x24, 0x24, 0x38, 0x24, 0x24, 0x38};
const byte char_A[] =           {0x00, 0x18, 0x24, 0x24, 0x3c, 0x24, 0x24, 0x24};
const byte char_c_Accented[] =  {0x04, 0x08, 0x18, 0x24, 0x20, 0x20, 0x24, 0x18};
const byte char_P[] =           {0x00, 0x38, 0x24, 0x24, 0x38, 0x20, 0x20, 0x20};
const byte char_i[] =           {0x00, 0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08};
const byte char_S[] =           {0x00, 0x1c, 0x20, 0x20, 0x18, 0x04, 0x04, 0x38};

// A message that only Poles can understand :-)
const byte* theMessage[] = {char_J, char_E, char_B, char_A, char_c_Accented, char_P, char_i, char_S, NULL};


void setup() 
{
  // put your setup code here, to run once:
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
}

// Display a single character
void displayMatrix(int addr, const byte rows[], byte dispMode = DISP_MODE_APPEAR)
{
  byte row = 0;
  byte col = 0;
  
  switch (dispMode)
  {
    case DISP_MODE_SLIDE_LEFT:
      for (row = 0; row < NUM_ROWS; row++)
      {
        for (col = 0; col < NUM_COLS; col++)
        {
          byte bitmap = rows[row] >> (NUM_COLS + col - 1);
          lc.setRow(addr, row, bitmap);
          delay(100);
          lc.setRow(addr, row, 0);
        } 
      }
      break;
    case DISP_MODE_APPEAR:
    default:
      for (row = 0; row < NUM_ROWS; row++)
        lc.setRow(addr, row, rows[row]);
      break;
  }
}

// Display the entire string
void displayString(int addr, const byte* text[], unsigned int delayTime = DELAY_BETWEEN_CHARS, byte dispMode = DISP_MODE_APPEAR)
{
  const byte** ptr = text;
  while (*ptr != NULL)
  {
    displayMatrix(addr, *ptr, dispMode);
    delay(delayTime);
    lc.clearDisplay(addr);
    ptr++;
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  displayString(0, theMessage, DELAY_BETWEEN_CHARS, DISP_MODE_SLIDE_LEFT);
  displayMatrix(0, theGrid, DISP_MODE_APPEAR);
  delay(2 * DELAY_BETWEEN_CHARS);
}
