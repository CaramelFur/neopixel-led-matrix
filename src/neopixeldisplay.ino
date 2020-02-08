#include "allSettings.h"
#include "halt.h"
#include "print.h"
#include "sd.h"
#include "bmp.h"
#include "neo.h"

double finishAt = 0;
double frameFinishAt = 0;
uint16_t millisPerFrame = 500;

ConfigHolder *config;
AnimConfigHolder *animConfig;

void setup()
{
  Serial.begin(115200);
  delay(300);
  Serial.println("Hello There");

  Better_NeoMatrix *mtrx = InitDisplay();

  setHaltDisplay(mtrx);

  InitBmp(mtrx);

  initSD();

  config = getMainConfig();
  animConfig = getAnimConfig();

  Serial.println((*config).brightness);

  SetDisplayBrightness(config->brightness);

  if (config->testMode == 1)
    TestDisplay();
}

void loop()
{
  finishAt = millis() + animConfig->length * 1000;
  frameFinishAt = millis() + millisPerFrame;

  millisPerFrame = 1000 / animConfig->fps;

  do
  {
    while (!selectNextFrame())
    {
      UpdateDisplay();

      while (frameFinishAt > millis())
      {
        delay(1);
      }

      frameFinishAt = millis() + millisPerFrame;
    }
  } while (finishAt > millis());

  selectNextDirectory(config->random);
}
