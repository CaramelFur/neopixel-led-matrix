#include "Arduino.h"
#include "SdFat.h"
#include "allSettings.h"
#include "filefuncs.h"

#pragma once

typedef uint16_t ConfigUINT;

namespace ConfigManager {
  enum CFGStatus {
    success,
    verbose_no_config_present,  // There was no config file, so the defaults were used (not critical at all)
    error_invalid_config,       // Encountered invalid config file
    error_invalid_config_type,  // An invalid config type was passed
    error_config_no_open,       // Could not open the config file
    error_config_no_close,      // Could not close the config file
  };

  enum ConfigType { main, animation };

  struct ConfigHolder {
    ConfigUINT brightness;
    ConfigUINT random;
    ConfigUINT testMode;
  };

  struct AnimConfigHolder {
    ConfigUINT fps;
    ConfigUINT length;
  };

  struct VariableLocationStatus {
    CFGStatus status;
    ConfigUINT* location;
  };

  namespace __internal {
    bool readFileUntil(SdFile* file, char* buf, uint8_t length, char delimiter);
  }

  ConfigHolder* getMainConfig();
  AnimConfigHolder* getAnimConfig();

  CFGStatus readConfigFile(SdFile* directory, ConfigType type);
}  // namespace ConfigManager
