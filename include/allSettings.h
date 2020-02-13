#include "Arduino.h"

#pragma once

// Enable this to debug
 #define SERIAL_DEBUG 1
// #undef SERIAL_DEBUG
// Enable this to debug sorting
// #define SORT_DEBUG 1
#undef SORT_DEBUG

static const uint8_t displayWidth = 16;
static const uint8_t displayHeight = 16;

static const uint8_t dispCharWidth = 5;
static const uint8_t dispCharHeight = 8;

static const uint8_t NeoPixelPin = D3;
static const bool IsGRB = true;

static const uint8_t hiddenCharsLen = 2;
static const char hiddenChars[hiddenCharsLen] = {'_', '.'};

static const uint8_t scrollTextBufferLength = 255;

extern const uint16_t textYPos;