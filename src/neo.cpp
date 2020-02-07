#include "neo.h"

const uint16_t textYPos = displayHeight / 2 - dispCharHeight / 2;

const uint16_t RED = rgbTo16(255, 0, 0);
const uint16_t GREEN = rgbTo16(0, 255, 0);
const uint16_t BLUE = rgbTo16(0, 0, 255);
const uint16_t WHITE = rgbTo16(255, 255, 255);

const uint8_t MatrixConf =
    NEO_MATRIX_BOTTOM +
    NEO_MATRIX_RIGHT + // The neo pixel strip starts in the top right
    NEO_MATRIX_ROWS +  // The neo pixel strip is divided into rows
    NEO_MATRIX_ZIGZAG; // The rows are attached in a zigzag pattern

const uint8_t PixelConf =
    NEO_KHZ800 +
            IsGRB
        ? NEO_GRB
        : NEO_RGB;

Better_NeoMatrix matrix = Better_NeoMatrix(displayWidth, displayHeight, NeoPixelPin, MatrixConf, PixelConf);

uint16_t rgbTo16(uint8_t r, uint8_t g, uint8_t b)
{
  r = r / 0xFF * 0b00011111;
  g = g / 0xFF * 0b00111111;
  b = b / 0xFF * 0b00011111;

  uint16_t output = r << 6;
  output = (output | g) << 5;
  output = output | b;

  return output;
}

Better_NeoMatrix *InitDisplay()
{
  // Todo: return displaybuffer with length

  matrix.begin();
  matrix.setBrightness(20);
  matrix.setTextWrap(false);
  matrix.setTextColor(WHITE);
  matrix.setCursor(0, displayHeight / 2 - dispCharHeight / 2);
  matrix.clear();

  return &matrix;
}

void SetDisplayBrightness(uint8_t brightness)
{
  matrix.setBrightness(brightness);
}

void UpdateDisplay()
{
  matrix.show();
  matrix.clear();
}

void TestDisplay()
{
  Serial.println("Testing display");
  for (uint8_t i = 0; i < 5; i++)
  {
    matrix.fillScreen(RED);
    matrix.show();
    delay(1000);
    matrix.fillScreen(GREEN);
    matrix.show();
    delay(1000);
    matrix.fillScreen(BLUE);
    matrix.show();
    delay(1000);
  }
  Serial.println("Done testing");
}
