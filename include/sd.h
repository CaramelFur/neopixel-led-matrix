#include <SPI.h>

#include "SdFat.h"
#include "allSettings.h"
#include "sdios.h"
#include "sort.h"

#pragma once

typedef uint8_t FileCountUInt;

static const FileCountUInt MaxFilesInFolder = 255;
static const FileCountUInt MaxFolders = 255;

static const uint8_t MaxFileNameLength = 13;

namespace SDReader {
  enum SDStatus {
    success,                    // Success
    error_init,                 // Could not open / initialize sd card
    error_root_no_open,         // Could not open the root of the sd card
    error_folder_no_open,       // Could not open a folder
    error_file_no_open,         // Could not open a file
    error_root_closed,          // Expected root directory to be open, but it was closed
    error_folder_closed,        // Expected directory to be open, but it was closed
    error_file_closed,          // Expected file to be open, but it was closed
    error_folder_not_get_name,  // It was unable to retrieve the name of a folder
    error_folder_no_close,      // Could not close folder
    error_file_no_get_name,     // Could not get the name of a file
    error_file_no_close,        // Could not close file
    error_non_folder_in_root,   // Found a non-folder file type in root, this is not good
    error_non_file_in_folder,   // Found a non-file file type in a folder
    error_too_many_files,       // There are too many files in a single directory for the esp to handle
    error_no_files,             // There were no
  };

  struct DirListStatus {
    SDStatus status;
    FileCountUInt amountOfFiles;
  };

  struct NextFrameStatus {
    SDStatus status;
    bool hasReachedEnd;
  };

  struct SdFileStatus {
    SDStatus status;
    SdFile* file;
  };

  namespace __internal {
    bool isHiddenChar(char toTest);
    void initSDVariables();
    SDStatus selectNextDirectory(uint16 folderIndexList[],
                                 FileCountUInt folderIndexListLength,
                                 FileCountUInt* currentIndexListIndex,
                                 SdFile* rootDir,
                                 SdFile* currentDir,
                                 SdFile* currentFile,
                                 SdFile* currentSubFile);
    NextFrameStatus selectNextFrame(char* fileNameList[],
                                    FileCountUInt fileNameListLength,
                                    FileCountUInt* currentIndex,
                                    SdFile* currentDir,
                                    SdFile* currentFile,
                                    bool stopOnLast);
    DirListStatus listAllFolders(uint16_t folderIndexList[], SdFile* rootDir, SdFile* currentFolder);
    DirListStatus listAllFiles(char* fileNameList[], SdFile* currentDir, SdFile* currentFile, bool allowFolders);

  }  // namespace

  SdFileStatus getRootDirectory();
  SdFileStatus getCurrentFile();
  SDStatus initialize();
  SDStatus selectNextDirectory(bool useRandomOrder);
  NextFrameStatus selectNextFrame();
}  // namespace SDReader
