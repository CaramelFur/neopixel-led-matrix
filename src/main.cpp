#include "main.h"

double finishAt = 0;
double frameFinishAt = 0;
uint16_t millisPerFrame = 500;

ConfigManager::ConfigHolder* config;
ConfigManager::AnimConfigHolder* animConfig;

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("Hello There");

  Better_NeoMatrix* mtrx = NeoDisplay::Initialize();
  setHaltDisplay(mtrx);

  BmpReader::Initialize(mtrx);

  SDReader::Initialize();

  loadMainConfig();

  NeoDisplay::SetDisplayBrightness(config->brightness);

  if (config->testMode == 1)
    NeoDisplay::TestDisplay();
}

void loadMainConfig() {
  SDReader::SdFileStatus rootDir = SDReader::getRootDirectory();
  if (rootDir.status != SDReader::SDStatus::success) {
    halt("Sd error: " + rootDir.status);
  }
  ConfigManager::readConfigFile(rootDir.file, ConfigManager::ConfigType::main);
  config = ConfigManager::getMainConfig();
}

void loadNextDirectory() {
  SDReader::SDStatus dirStatus = SDReader::selectNextDirectory(config->random);
  if (dirStatus != SDReader::SDStatus::success) {
    halt("Sd error: " + dirStatus);
  }
  SDReader::SdFileStatus curDir = SDReader::getCurrentDirectory();
  if (curDir.status != SDReader::SDStatus::success) {
    halt("Sd error: " + curDir.status);
  }
  ConfigManager::readConfigFile(curDir.file, ConfigManager::ConfigType::animation);
  animConfig = ConfigManager::getAnimConfig();
}

SdFile* loadNextFile() {
  SDReader::NextFileStatus fileStatus = SDReader::selectNextFile();
  if (fileStatus.status != SDReader::SDStatus::success) {
    halt("Sd error: " + fileStatus.status);
  }
  SDReader::SdFileStatus curFile = SDReader::getCurrentFile();
  if (curFile.status != SDReader::SDStatus::success) {
    halt("Sd error: " + curFile.status);
  }
  return curFile.file;
}

void loop() {
  finishAt = millis() + animConfig->length * 1000;
  frameFinishAt = millis() + millisPerFrame;

  millisPerFrame = 1000 / animConfig->fps;

  do {
    do {
      ShowDisplayUpdates();

      while (frameFinishAt > millis()) {
        delay(1);
      }

      frameFinishAt = millis() + millisPerFrame;
    } while (!selectNextFrame());
  } while (finishAt > millis());

  selectNextDirectory(config->random);
}
