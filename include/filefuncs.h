#include "Arduino.h"
#include "SdFat.h"

#pragma once

String getFileName(SdFile * file, uint8_t buflength = 13);