#include "filefuncs.h"

String getFileName(SdFile * file, uint8_t buflength){
  char * out = new char[buflength];
  if(!file->getName(out, buflength)){
    out = "error";
  }

  return String(out);
}