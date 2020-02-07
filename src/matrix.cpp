#include "matrix.h"

Better_NeoMatrix::Better_NeoMatrix(
    int w, int h, uint8_t pin,
    uint8_t matrixType, neoPixelType ledType)
    : Adafruit_NeoMatrix(w, h, pin,
                         matrixType, ledType) {}

uint16_t Better_NeoMatrix::getPixelsLength()
{
  return numBytes;
}

void Better_NeoMatrix::drawRGBPixel(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b)
{
  /*r = r / 255 * brightness;
  g = g / 255 * brightness;
  b = b / 255 * brightness;*/
  setPassThruColor((r << 16) | (g << 8) | b);
  drawPixel(x, y, 0);
  setPassThruColor();
}