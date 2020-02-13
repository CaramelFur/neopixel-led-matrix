#include "Arduino.h"
#include "SdFat.h"

#pragma once

namespace SD {
  enum ReadFileUntilStatus {
    success,
    warn_overflow,
    error_read,
    error_overflow,
  };

  struct ReadFileUntilResult {
    ReadFileUntilStatus status;
    uint16_t textRead;
  };

  String getFileName(SdFile* file, uint8_t buflength = 13);
  uint16_t readFileUntil(SdFile* file, char* buf, uint8_t length, char delimiter);
  ReadFileUntilResult readFileUntilAdv(SdFile* file, char* buf, uint8_t length, char delimiter);
}  // namespace SD
