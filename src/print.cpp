#include "print.h"

#ifdef SERIAL_DEBUG

void printByte(byte inByte) {
  for (int b = 7; b >= 0; b--) {
    Serial.print(bitRead(inByte, b));
  }
}

void printHalfByte(byte inByte, bool high = false) {
  for (int b = (high ? 7 : 3); b >= (high ? 4 : 0); b--) {
    Serial.print(bitRead(inByte, b));
  }
}

void printPixelArray(byte pixelArray[], uint8_t width, uint8_t height) {
  for (uint8_t h = 0; h < height; h++) {
    for (uint8_t w = 0; w < width; w++) {
      for (uint8_t clr = 0; clr < 3; clr++) {
        byte pix = pixelArray[h * width * 3 + w * 3 + clr];
        if (pix <= 0xF)
          Serial.print("0");
        Serial.print(pix, HEX);
      }
      Serial.print(" ");
    }
    Serial.println();
  }
}

#endif