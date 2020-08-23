#include "main.h"

double finishAt = 0;
double frameFinishAt = 0;
uint16_t millisPerFrame = 500;

Better_NeoMatrix* mtrx;

ConfigManager::ConfigHolder* mainConfig;
ConfigManager::AnimConfigHolder* animConfig;

void setup() {
  delay(300);

#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  Serial.println("Hello World!");
#endif

  mtrx = NeoDisplay::Initialize();
  Halt::setDisplay(mtrx);

  BmpReader::Initialize(mtrx);

  isSuccess(SDReader::Initialize());

  loadMainConfig();

  NeoDisplay::SetDisplayBrightness(mainConfig->brightness);

  if (mainConfig->testMode == 1)
    NeoDisplay::TestDisplay();
}

void loop() {
  loadNextDirectory();

  if (animConfig->type == 0) {
    displayCurrentAnimation();
  } else if (animConfig->type == 1) {
    displayCurrentScrollText();
  } else {
    Halt::halt("Unsupported animation type");
  }
}

void loadMainConfig() {
  SDReader::SdFileStatus rootDir = SDReader::getRootDirectory();
  isSuccess(rootDir.status);
  ConfigManager::readConfigFile(rootDir.file, ConfigManager::ConfigType::main);
  mainConfig = ConfigManager::getMainConfig();
}

void loadNextDirectory() {
  isSuccess(SDReader::selectNextDirectory(mainConfig->random));

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

      NeoDisplay::Flush();

      while (frameFinishAt > millis())
        ;

      if (nextFrameStatus.hasReachedEnd)
        break;
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