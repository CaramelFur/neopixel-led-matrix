#include "Arduino.h"
#include "SdFat.h"

#pragma once

namespace SD {
  String getFileName(SdFile* file, uint8_t buflength = 13);
  bool readFileUntil(SdFile* file, char* buf, uint8_t length, char delimiter);
}  // namespace SD
