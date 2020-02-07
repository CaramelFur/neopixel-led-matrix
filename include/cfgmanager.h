#include "Arduino.h"
#include "SdFat.h"
#include "allSettings.h"
#include "halt.h"
#include "filefuncs.h"

#pragma once

struct ConfigHolder {
  uint16_t brightness;
  uint16_t random;
  uint16_t testMode;
};

struct AnimConfigHolder {
  uint16_t fps;
  uint16_t length;
};

enum ConfigType {
  main,
  animation
};

bool readConfigFile(SdFile *directory, ConfigType type);

ConfigHolder* getMainConfig();

AnimConfigHolder* getAnimConfig();
