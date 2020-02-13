#include "matrix.h"

Better_NeoMatrix::Better_NeoMatrix(int w, int h, uint8_t pin, uint8_t matrixType, neoPixelType ledType)
    : Adafruit_NeoMatrix(w, h, pin, matrixType, ledType) {}

uint16_t Better_NeoMatrix::getPixelsLength() {
  return numBytes;
}

void Better_NeoMatrix::drawRGBPixel(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPassThruColor((r << 16) | (g << 8) | b);
  drawPixel(x, y, 0);
  setPassThruColor();
}

void Better_NeoMatrix::scrollText(char* text, uint16_t length, uint8_t speed) {
  uint32_t fullLengthInPixels = length * (dispCharWidth + 1) + displayWidth;

  for (uint32_t i = 0; i < fullLengthInPixels; i++) {
    setCursor(-i + displayWidth, textYPos);
    clear();
    print(text);
    show();
    delay(speed);
    if (i == 0)
      delay(500);
  }
}