#include "main.h"

double finishAt = 0;
double frameFinishAt = 0;
uint16_t millisPerFrame = 500;

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

  Better_NeoMatrix* mtrx = NeoDisplay::Initialize();
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
    halt("nigg");
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

      SDReader::SdFileStatus curFrame = SDReader::getCurrentFile();
      isSuccess(curFrame.status);

      isSuccess(BmpReader::readBmp(curFrame.file));

      NeoDisplay::PushFrameOut();

      while (frameFinishAt > millis())
        ;

      if (nextFrameStatus.hasReachedEnd)
        break;
    }
  } while (finishAt > millis());
}