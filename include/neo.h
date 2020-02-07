#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "allSettings.h"
#include "matrix.h"
#include "halt.h"

#pragma once

struct PixelBuffer
{
  uint16_t length;
  uint8_t *pixels;
};

uint16_t rgbTo16(uint8_t r, uint8_t g, uint8_t b);

Better_NeoMatrix *InitDisplay();

void SetDisplayBrightness(uint8_t brightness);

void TestDisplay();

void UpdateDisplay();