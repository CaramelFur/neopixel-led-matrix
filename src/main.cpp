#include "main.h"

double finishAt = 0;
double frameFinishAt = 0;
uint16_t millisPerFrame = 500;

Better_NeoMatrix* mtrx;

ConfigManager::ConfigHolder* config;
ConfigManager::AnimConfigHolder* animConfig;

void setup() {
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
#endif
  delay(300);
#ifdef SERIAL_DEBUG
  Serial.println("Hello World!");
#endif

  mtrx = NeoDisplay::Initialize();
  setHaltDisplay(mtrx);

  BmpReader::Initialize(mtrx);

  isSuccess(SDReader::Initialize());

  loadMainConfig();

  NeoDisplay::SetDisplayBrightness(config->brightness);

  if (config->testMode == 1)
    NeoDisplay::TestDisplay();
}

void loop() {
  loadNextDirectory();

  if (animConfig->type == 0) {
    displayCurrentAnimation();
  } else if (animConfig->type == 1) {
    displayCurrentScrollText();
  } else {
    halt("Unsupported animation type");
  }
}

void loadMainConfig() {
  SDReader::SdFileStatus rootDir = SDReader::getRootDirectory();
  isSuccess(rootDir.status);
  ConfigManager::readConfigFile(rootDir.file, ConfigManager::ConfigType::main);
  config = ConfigManager::getMainConfig();
}

void loadNextDirectory() {
  isSuccess(SDReader::selectNextDirectory(config->random));

  SDReader::SdFileStatus curDir = SDReader::getCurrentDirectory();
  isSuccess(curDir.status);

  ConfigManager::readConfigFile(curDir.file, ConfigManager::ConfigType::animation);
  animConfig = ConfigManager::getAnimConfig();
}

void displayCurrentAnimation() {
  finishAt = millis() + animConfig->length * 1000;
  millisPerFrame = 1000 / animConfig->fps;

  do {
    while (true) {
      frameFinishAt = millis() + millisPerFrame;
      SDReader::NextFileStatus nextFrameStatus = SDReader::selectNextFile();
      isSuccess(nextFrameStatus.status);

      if (nextFrameStatus.hasReachedEnd)
        break;

      SDReader::SdFileStatus curFrame = SDReader::getCurrentFile();
      isSuccess(curFrame.status);

      isSuccess(BmpReader::readBmp(curFrame.file));

      NeoDisplay::PushFrameOut();

      while (frameFinishAt > millis())
        ;
    }
  } while (finishAt > millis());
}

void displayCurrentScrollText() {
  char charBuffer[scrollTextBufferLength] = {};
  
  if (animConfig->length == 0)
    animConfig->length = 1;

  for (ConfigUINT i = 0; i < animConfig->length; i++) {
    while (true) {
      SDReader::NextFileStatus nextFrameStatus = SDReader::selectNextFile();
      isSuccess(nextFrameStatus.status);

      if (nextFrameStatus.hasReachedEnd)
        break;

      SDReader::SdFileStatus curFrame = SDReader::getCurrentFile();
      isSuccess(curFrame.status);

      while (true) {
        SD::ReadFileUntilResult result = SD::readFileUntilAdv(curFrame.file, charBuffer, scrollTextBufferLength, '\n');
        if (result.status == SD::ReadFileUntilStatus::error_read)
          break;

        mtrx->scrollText(charBuffer, result.textRead, animConfig->fps);
      }
    }
  }
}