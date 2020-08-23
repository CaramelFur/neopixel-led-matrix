#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "allSettings.h"
#include "matrix.h"

#pragma once

#define rgbTo16Compile(r, g, b) (((((r / 0xFF * 0b00011111) << 6) | (g / 0xFF * 0b00111111)) << 5) | (b / 0xFF * 0b00011111))

class NeoDisplay {
 private:
  const static uint8_t MatrixConf = NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +  // The neo pixel strip starts in the top right
                                    NEO_MATRIX_ROWS +                       // The neo pixel strip is divided into rows
                                    NEO_MATRIX_ZIGZAG;                      // The rows are attached in a zigzag pattern
  const static uint8_t PixelConf = NEO_KHZ800 + IsGRB ? NEO_GRB : NEO_RGB;

  static Better_NeoMatrix* matrix;

 public:
  static Better_NeoMatrix* Initialize();

  static void SetDisplayBrightness(uint8_t brightness);

  static void TestDisplay();

  static void Flush();

  class Colors;
};

class NeoDisplay::Colors {
  // private:
  // static uint16_t rgbTo16(uint8_t r, uint8_t g, uint8_t b);

 public:
  const static uint16_t red = rgbTo16Compile(255, 0, 0);
  const static uint16_t green = rgbTo16Compile(0, 255, 0);
  const static uint16_t blue = rgbTo16Compile(0, 0, 255);
  const static uint16_t white = rgbTo16Compile(255, 255, 255);
  const static uint16_t black = rgbTo16Compile(0, 0, 0);
};

#undef rgbTo16Compile