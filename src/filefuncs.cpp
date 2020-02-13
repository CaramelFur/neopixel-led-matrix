#include "filefuncs.h"

namespace SD {
  String getFileName(SdFile* file, uint8_t buflength) {
    char* out = new char[buflength];
    if (!file->getName(out, buflength)) {
      out = "error";
    }

    return String(out);
  }

  uint8_t readFileUntil(SdFile* file, char* buf, uint8_t length, char delimiter) {
    uint8_t currentCharPos = 0;

    // Keep going as long as we dont go outside of the buffer
    while (currentCharPos != length) {
      if (!file->read(&buf[currentCharPos], 1)) {
        // Quit if it reaches the end of the file
        break;
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
        return currentCharPos + 1;
      }
      currentCharPos++;
    }
    return 0;
  }
}  // namespace SD