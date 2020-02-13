#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "allSettings.h"
#include "matrix.h"

#pragma once

namespace NeoDisplay {
  namespace colors {
    extern const uint16_t red;
    extern const uint16_t green;
    extern const uint16_t blue;
    extern const uint16_t white;
    extern const uint16_t black;
  }  // namespace colors

  namespace __internal {
    uint16_t rgbTo16(uint8_t r, uint8_t g, uint8_t b);
  }  // namespace __internal

  Better_NeoMatrix* Initialize();

  void SetDisplayBrightness(uint8_t brightness);

  void TestDisplay();

  void PushFrameOut();

}  // namespace NeoDisplay
