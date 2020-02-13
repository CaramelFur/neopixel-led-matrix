#include "allSettings.h"
#include "bmp.h"
#include "cfgmanager.h"
#include "halt.h"
#include "sd.h"

#pragma once

bool isSuccess(BmpReader::BMPStatus status);

bool isSuccess(ConfigManager::CFGStatus status);

bool isSuccess(SDReader::SDStatus status);