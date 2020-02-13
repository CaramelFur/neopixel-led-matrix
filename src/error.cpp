#include "error.h"

bool isSuccess(BmpReader::BMPStatus status) {
  if (status == BmpReader::BMPStatus::success)
    return true;
  halt("BMP error: " + String(status));
  return false;
}

bool isSuccess(ConfigManager::CFGStatus status) {
  if (status == ConfigManager::CFGStatus::success)
    return true;
  halt("CFG error: " + String(status));
  return false;
}

bool isSuccess(SDReader::SDStatus status) {
  if (status == SDReader::SDStatus::success)
    return true;
  halt("SD error: " + String(status));
  return false;
}
