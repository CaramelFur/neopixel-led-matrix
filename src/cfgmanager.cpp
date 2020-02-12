#include "cfgmanager.h"

// Must start with hiddenChar to not crash

namespace ConfigManager {
  namespace __internal {
    const char* configFileName = "_config";

    const uint8_t tempReaderLength = 20;

    const ConfigHolder defaultConfig = {
        .brightness = 50,
        .random = 0,
        .testMode = 0,
    };

    const AnimConfigHolder defaultAnimConfig = {
        .fps = 2,
        .length = 10,
    };

    char tempReader[tempReaderLength] = {};

    SdFile configFile;

    ConfigHolder config = defaultConfig;
    AnimConfigHolder animConfig = defaultAnimConfig;

    bool readFileUntil(SdFile* file, char* buf, uint8_t length, char delimiter) {
      uint8_t currentCharPos = 0;

      // Keep going as long as we dont go outside of the buffer
      while (currentCharPos != length) {
        if (!file->read(&buf[currentCharPos], 1)) {
          // Quit if it reaches the end of the file
          break;
        }

        if (buf[currentCharPos] == delimiter ||                        // If the current Char is the deliter stop
            (delimiter == '\n' ? buf[currentCharPos] == '\r' : false)  // If the delimiter is \n also check for \r
        ) {
          // If the current \r is surpassed by a \n process that too
          if (buf[currentCharPos] == '\r' && file->peek() == '\n') {
            file->read();
          }

          // End the string with a null character and return
          buf[currentCharPos] = '\0';
          return true;
        }
        currentCharPos++;
      }
      return false;
    }

    VariableLocationStatus getVariableLocation(char* name, ConfigType type) {
      VariableLocationStatus out = {CFGStatus::success, 0};

      switch (type) {
        case ConfigType::main:
          if (strcmp(name, "bri") == 0)
            out.location = &config.brightness;
          else if (strcmp(name, "rnd") == 0)
            out.location = &config.random;
          else if (strcmp(name, "tst") == 0)
            out.location = &config.testMode;
          else
            out.status = CFGStatus::error_invalid_config;
          break;
        case ConfigType::animation:
          if (strcmp(name, "fps") == 0)
            out.location = &animConfig.fps;
          else if (strcmp(name, "lnt") == 0)
            out.location = &animConfig.length;
          else
            out.status = CFGStatus::error_invalid_config;
          break;

        default:
          out.status = CFGStatus::error_invalid_config_type;
          break;
      }

      return out;
    }
  }  // namespace __internal

  using namespace __internal;

  ConfigHolder* getMainConfig() { return &config; }

  AnimConfigHolder* getAnimConfig() { return &animConfig; }

  CFGStatus readConfigFile(SdFile* directory, ConfigType type) {
    switch (type) {
      case ConfigType::main:
        config = defaultConfig;
        break;
      case ConfigType::animation:
        animConfig = defaultAnimConfig;
        break;
    }

    if (!directory->exists(configFileName))
      return CFGStatus::verbose_no_config_present;

    if (!configFile.open(directory, configFileName, O_RDONLY))
      return CFGStatus::error_config_no_open;

    while (readFileUntil(&configFile, tempReader, tempReaderLength, ' ')) {
      VariableLocationStatus varLocation = getVariableLocation(tempReader, type);

      if (varLocation.status != CFGStatus::success) {
        return varLocation.status;
      }

      if (!readFileUntil(&configFile, tempReader, tempReaderLength, '\n'))
        return CFGStatus::error_invalid_config;

      *varLocation.location = atoi(tempReader);
    }

    if (!configFile.close())
      return CFGStatus::error_config_no_close;

    return CFGStatus::success;
  }

}  // namespace ConfigManager
