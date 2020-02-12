#include "allSettings.h"

#pragma once

#ifdef SERIAL_DEBUG

void printByte(byte inByte);

void printHalfByte(byte inByte, bool high);

void printPixelArray(byte pixelArray[], uint8_t width, uint8_t height);

#endif