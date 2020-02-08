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

typedef uint8_t FileCountUInt;

void initSD();

bool selectNextFrame();
void selectNextDirectory(bool doRandom);

static const FileCountUInt MaxFilesInFolder = 255;
static const FileCountUInt MaxFolders = 255;

static const uint8_t MaxFileNameLength = 13;