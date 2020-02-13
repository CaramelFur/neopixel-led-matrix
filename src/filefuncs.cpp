#include "filefuncs.h"

namespace SD {

  String getFileName(SdFile* file, uint8_t buflength) {
    char* out = new char[buflength];
    if (!file->getName(out, buflength)) {
      out = (char*)"Error";
    }

    return String(out);
  }

  uint16_t readFileUntil(SdFile* file, char* buf, uint8_t length, char delimiter) {
    return readFileUntilAdv(file, buf, length, delimiter).textRead;
  }

  ReadFileUntilResult readFileUntilAdv(SdFile* file, char* buf, uint8_t length, char delimiter) {
    uint16_t currentCharPos = 0;

    // Keep going as long as we dont go outside of the buffer
    while (currentCharPos != length) {
      if (!file->read(&buf[currentCharPos], 1)) {
        // Quit if it reaches the end of the file
        return ReadFileUntilResult{
            ReadFileUntilStatus::error_read,
            0,
        };
      }

      if (buf[currentCharPos] == delimiter ||                        // If the current Char is the deliter stop
          (delimiter == '\n' ? buf[currentCharPos] == '\r' : false)  // If the delimiter is \n also check for \r
      ) {
        // If the current \r is surpassed by a \n process that too
        if (buf[currentCharPos] == '\r' && file->peek() == '\n') {
          file->read();
        }

        // End the string with a null character and return
        buf[currentCharPos] = '\0';

        currentCharPos = currentCharPos + 1;

        return ReadFileUntilResult{
            ReadFileUntilStatus::success,
            currentCharPos,
        };
      }

      if (currentCharPos == length - 2) {
        buf[currentCharPos + 1] = '\0';
        currentCharPos = currentCharPos + 2;
        return ReadFileUntilResult{
            ReadFileUntilStatus::warn_overflow,
            currentCharPos,
        };
      }
      currentCharPos++;
    }

    return ReadFileUntilResult{
        ReadFileUntilStatus::error_overflow,
        0,
    };
  }
}  // namespace SD