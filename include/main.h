#include "allSettings.h"
#include "bmp.h"
#include "cfgmanager.h"
#include "halt.h"
#include "neo.h"
#include "print.h"
#include "sd.h"

#pragma once

void loadMainConfig();
void loadNextDirectory();
SdFile* loadNextFile();