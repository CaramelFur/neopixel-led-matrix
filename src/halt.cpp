#include "halt.h"

Better_NeoMatrix* Halt::errorDisplay{
  0
};

void Halt::printToDisplay(String message, bool onlyOnce) {
  do {
    errorDisplay->scrollText(message.begin(), message.length(), scrollSpeed);
  } while (!onlyOnce);
}

// Stops execution and gives an error message
void Halt::halt(String message, bool onlyOnce) {
#ifdef SERIAL_DEBUG
  Serial.print("Arduino halted with: ");
  Serial.println(message);
#endif

  if (errorDisplay != 0) {
    printToDisplay(message, onlyOnce);
  } else {
    do {
      delay(10000);
#ifdef SERIAL_DEBUG
      Serial.print(".");
#endif
    } while (!onlyOnce);
  }
}

void Halt::setDisplay(Better_NeoMatrix* mat) {
  errorDisplay = mat;
}