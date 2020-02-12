#include "Arduino.h"

#pragma once

// Enable this to debug
#define SERIAL_DEBUG 1

static const uint8_t displayWidth = 16;
static const uint8_t displayHeight = 16;

static const uint8_t dispCharWidth = 5;
static const uint8_t dispCharHeight = 8;

static const uint8_t NeoPixelPin = D3;
static const bool IsGRB = true;

static const uint8_t hiddenCharsLen = 2;
static const char hiddenChars[hiddenCharsLen] = {'_', '.'};

extern const char *configFileName; 

extern const uint16_t textYPos;