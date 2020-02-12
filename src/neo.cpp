#include "neo.h"

const uint16_t textYPos = displayHeight / 2 - dispCharHeight / 2;

namespace NeoDisplay {
  namespace __internal {
    const uint8_t MatrixConf = NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +  // The neo pixel strip starts in the top right
                               NEO_MATRIX_ROWS +                       // The neo pixel strip is divided into rows
                               NEO_MATRIX_ZIGZAG;                      // The rows are attached in a zigzag pattern
    const uint8_t PixelConf = NEO_KHZ800 + IsGRB ? NEO_GRB : NEO_RGB;

    uint16_t rgbTo16(uint8_t r, uint8_t g, uint8_t b) {
      r = r / 0xFF * 0b00011111;
      g = g / 0xFF * 0b00111111;
      b = b / 0xFF * 0b00011111;

      uint16_t output = r << 6;
      output = (output | g) << 5;
      output = output | b;

      return output;
    }

    Better_NeoMatrix matrix = Better_NeoMatrix(displayWidth, displayHeight, NeoPixelPin, MatrixConf, PixelConf);
  }  // namespace __internal

  using namespace __internal;

  namespace colors {
    const uint16_t red = rgbTo16(255, 0, 0);
    const uint16_t green = rgbTo16(0, 255, 0);
    const uint16_t blue = rgbTo16(0, 0, 255);
    const uint16_t white = rgbTo16(255, 255, 255);
    const uint16_t black = rgbTo16(0, 0, 0);
  }  // namespace colors

  Better_NeoMatrix* Initialize() {
    matrix.begin();
    matrix.setBrightness(20);
    matrix.setTextWrap(false);
    matrix.setTextColor(colors::white);
    matrix.setCursor(0, displayHeight / 2 - dispCharHeight / 2);
    matrix.clear();

    return &matrix;
  }

  void SetDisplayBrightness(uint8_t brightness) { matrix.setBrightness(brightness); }

  void PushFrameOut() {
    matrix.show();
    matrix.clear();
  }

  void TestDisplay() {
#ifdef SERIAL_DEBUG
    Serial.println("Testing display");
#endif
    for (uint8_t i = 0; i < 5; i++) {
      matrix.fillScreen(colors::red);
      matrix.show();
      delay(1000);
      matrix.fillScreen(colors::green);
      matrix.show();
      delay(1000);
      matrix.fillScreen(colors::blue);
      matrix.show();
      delay(1000);
    }
#ifdef SERIAL_DEBUG
    Serial.println("Done testing");
#endif
  }

}  // namespace NeoDisplay
