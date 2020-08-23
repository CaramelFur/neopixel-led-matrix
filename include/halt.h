#include "Arduino.h"
#include "matrix.h"
#include "allSettings.h"

#pragma once

class Halt
{
  private:
    static const uint8_t scrollSpeed = 50;
    static Better_NeoMatrix* errorDisplay;

    static void printToDisplay(String message, bool onlyOnce);

  public:
    static void halt(String message, bool onlyOnce = false);
    static void setDisplay(Better_NeoMatrix * mat);
};