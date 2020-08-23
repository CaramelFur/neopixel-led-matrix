#include "neo.h"

const uint16_t textYPos = displayHeight / 2 - dispCharHeight / 2;

Better_NeoMatrix* NeoDisplay::matrix{
  0
};

Better_NeoMatrix* NeoDisplay::Initialize() {
  matrix = new Better_NeoMatrix(displayWidth, displayHeight, NeoPixelPin, MatrixConf, PixelConf);

  matrix->begin();
  matrix->setBrightness(20);
  matrix->setTextWrap(false);
  matrix->setTextColor(Colors::white);
  matrix->setCursor(0, displayHeight / 2 - dispCharHeight / 2);
  matrix->clear();

  return matrix;
}

void NeoDisplay::SetDisplayBrightness(uint8_t brightness) {
  matrix->setBrightness(brightness);
}

void NeoDisplay::Flush() {
  matrix->show();
  matrix->clear();
}

void NeoDisplay::TestDisplay() {
#ifdef SERIAL_DEBUG
  Serial.println("Testing display");
#endif
  for (uint8_t i = 0; i < 5; i++) {
    matrix->fillScreen(Colors::red);
    matrix->show();
    delay(1000);
    matrix->fillScreen(Colors::green);
    matrix->show();
    delay(1000);
    matrix->fillScreen(Colors::blue);
    matrix->show();
    delay(1000);
  }
#ifdef SERIAL_DEBUG
  Serial.println("Done testing");
#endif
}

/*
uint16_t NeoDisplay::Colors::rgbTo16(uint8_t r, uint8_t g, uint8_t b) {
  r = r / 0xFF * 0b00011111;
  g = g / 0xFF * 0b00111111;
  b = b / 0xFF * 0b00011111;

  uint16_t output = r << 6;
  output = (output | g) << 5;
  output = output | b;

  return output;
}
*/
