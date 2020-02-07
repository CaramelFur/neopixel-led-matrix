#include "Arduino.h"
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include "halt.h"
#include "allSettings.h"
#include "bmp.h"
#include "sort.h"
#include "cfgmanager.h"

#pragma once

typedef uint16_t FileCountUInt;

void initSD();

void selectNextFrame();
void selectNextDirectory(bool doRandom);

static const FileCountUInt MaxFilesInFolder = 30;
static const FileCountUInt MaxFolders = 200;

static const uint8_t MaxFileNameLength = 13;