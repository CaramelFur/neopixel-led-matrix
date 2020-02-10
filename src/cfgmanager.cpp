#include "cfgmanager.h"

const char *configFileName = "_config"; // Must start with hiddenChar to not crash

const uint8_t tempReaderLength = 20;
static char tempReader[tempReaderLength] = {};

static SdFile configFile;

const ConfigHolder defaultConfig = {
    .brightness = 50,
    .random = 0,
    .testMode = 0,
};

const AnimConfigHolder defaultAnimConfig = {
    .fps = 2,
    .length = 10,
};

static ConfigHolder config = defaultConfig;

static AnimConfigHolder animConfig = defaultAnimConfig;

ConfigHolder *getMainConfig()
{
  return &config;
}

AnimConfigHolder *getAnimConfig()
{
  return &animConfig;
}

bool readFileUntil(SdFile *file, char *buf, uint8_t length, char delimiter)
{
  uint8_t i = 0;
  while (i != length)
  {
    if (!file->read(&buf[i], 1))
      break;
    if (buf[i] == delimiter || (delimiter == '\n' ? buf[i] == '\r' : false))
    {
      if (buf[i] == '\r' && file->peek() == '\n')
        file->read();
      buf[i] = '\0';
      return true;
    }
    i++;
  }
  return false;
}

uint16_t *getVariableLocation(char *name, ConfigType type)
{
  switch (type)
  {
  case main:
    if (strcmp(name, "bri") == 0)
      return &config.brightness;
    else if (strcmp(name, "rnd") == 0)
      return &config.random;
    else if (strcmp(name, "tst") == 0)
      return &config.testMode;
    else
      halt("Invalid config file, " + String(name) + " is no option");
    break;
  case animation:
    if (strcmp(name, "fps") == 0)
      return &animConfig.fps;
    else if (strcmp(name, "lnt") == 0)
      return &animConfig.length;
    else
      halt("Invalid config file, " + String(name) + " is no option");
    break;

  default:
    halt("Incorrect config type");
    break;
  }
  return 0;
}

bool readConfigFile(SdFile *directory, ConfigType type)
{
  if (type == animation)
  {
    animConfig = defaultAnimConfig;
  }
  else if (type == main)
  {
    config = defaultConfig;
  }

  if (!directory->exists(configFileName))
    return false;

  if (!configFile.open(directory, configFileName, O_RDONLY))
    halt("Could not open config file in dir: " + getFileName(directory));

  while (readFileUntil(&configFile, tempReader, tempReaderLength, ' '))
  {
    uint16_t *destination = getVariableLocation(tempReader, type);

    if (!readFileUntil(&configFile, tempReader, tempReaderLength, '\n'))
      halt("Invalid config file in dir: " + getFileName(directory));

    *destination = atoi(tempReader);

    Serial.println(*destination);
  }

  if (!configFile.close())
    halt("Could not close config file");

  return true;
}
