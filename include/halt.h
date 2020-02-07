#include "Arduino.h"
#include "matrix.h"
#include "allSettings.h"

#pragma once

void halt(String message, bool onlyOnce = false);

void setHaltDisplay(Better_NeoMatrix * mat);