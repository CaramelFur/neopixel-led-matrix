#include "Arduino.h"
#include <Adafruit_NeoMatrix.h>

#include "allSettings.h"

#pragma once

class Better_NeoMatrix : public Adafruit_NeoMatrix
{

public:
  // Constructor for single matrix:
  Better_NeoMatrix(int w, int h, uint8_t pin = 6,
                   uint8_t matrixType = NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS,
                   neoPixelType ledType = NEO_GRB + NEO_KHZ800);

  uint16_t getPixelsLength();

  void drawRGBPixel(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b);

  void scrollText(char *text, uint16_t length, uint8_t speed = 50);
};