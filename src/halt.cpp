#include "halt.h"

const uint8_t scrollSpeed = 50;

Better_NeoMatrix* errorDisplay = 0;

void printToDisplay(String message, bool onlyOnce) {
  do {
    errorDisplay->scrollText(message.begin(), message.length());
  } while (!onlyOnce);
}

// Stops execution and gives an error message
void halt(String message, bool onlyOnce) {
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

void setHaltDisplay(Better_NeoMatrix* mat) {
  errorDisplay = mat;
}