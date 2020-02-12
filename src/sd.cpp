#include "sd.h"

namespace SDReader {
  namespace __internal {  // Private
    // Sd holders
    SdFat sd;
    SdFile rootDirHolder;
    SdFile currentDirHolder;
    SdFile currentSubFileHolder;
    SdFile currentFileHolder;

    // File holders
    FileCountUInt currentFolderIndexesIndex = 0;
    FileCountUInt folderIndexesLength = 0;
    uint16_t folderIndexes[MaxFolders] = {};

    FileCountUInt currentFileNameIndex = 0;
    FileCountUInt fileNamesLength = 0;
    char* fileNames[MaxFilesInFolder] = {};

    FileCountUInt currentSubFileNameIndex = 0;
    FileCountUInt subFileNamesLength = 0;
    char* subFileNames[MaxFilesInFolder] = {};

    bool isHiddenChar(char toTest) {
      for (uint8_t i = 0; i < hiddenCharsLen; i++) {
        if (hiddenChars[i] == toTest)
          return true;
      }
      return false;
    }

    void initSDVariables() {
      FileCountUInt i;
      for (i = 0; i < MaxFilesInFolder; i++) {
        fileNames[i] = new char[MaxFileNameLength];
      }
      for (i = 0; i < MaxFilesInFolder; i++) {
        subFileNames[i] = new char[MaxFileNameLength];
      }
    }

    SDStatus selectNextDirectory(uint16 folderIndexList[],
                                 FileCountUInt folderIndexListLength,
                                 FileCountUInt* currentIndexListIndex,
                                 SdFile* rootDir,
                                 SdFile* currentDir,
                                 SdFile* currentFile,
                                 SdFile* currentSubFile) {
      if (currentDir->isOpen()) {
        if (!currentDir->close()) {
          return SDStatus::error_folder_no_close;
        }
      }

      if (currentSubFile->isOpen()) {
        if (!currentSubFile->close()) {
          return SDStatus::error_file_no_close;
        }
      }

      if (currentFile->isOpen()) {
        if (!currentFile->close()) {
          return SDStatus::error_file_no_close;
        }
      }

      *currentIndexListIndex = *currentIndexListIndex + 1;
      if (*currentIndexListIndex >= folderIndexListLength)
        *currentIndexListIndex = 0;

      if (!currentDir->open(rootDir, folderIndexList[*currentIndexListIndex], O_RDONLY)) {
        return SDStatus::error_folder_no_open;
      }

#ifdef DEBUG_MODE
      Serial.print("Entering directory: ");
      currentDir->printName();
      Serial.println();
#endif

      DirListStatus listStatus = listAllFiles(fileNames, currentDir, currentFile, true);
      if (listStatus.status != SDStatus::success) {
        return listStatus.status;
      }
      fileNamesLength = listStatus.amountOfFiles;
      currentFileNameIndex = -1;

      return SDStatus::success;
    }

    NextFrameStatus selectNextFrame(char* fileNameList[],
                                    FileCountUInt fileNameListLength,
                                    FileCountUInt* currentIndex,
                                    SdFile* currentDir,
                                    SdFile* currentFile,
                                    bool stopOnLast) {
      bool reachedEndOfFolder = false;
      if (currentFile->isOpen()) {
        if (!currentFile->close()) {
          return NextFrameStatus{SDStatus::error_file_no_close, reachedEndOfFolder};
        }
      }

#ifdef DEBUG_MODE
      Serial.print("Opening frame from: ");
      currentDir->printName();
      Serial.println();
#endif

      *currentIndex = *currentIndex + 1;
      if (*currentIndex == fileNameListLength) {
        *currentIndex = 0;
        reachedEndOfFolder = true;
      }

      if (stopOnLast && reachedEndOfFolder) {
        return NextFrameStatus{SDStatus::success, reachedEndOfFolder};
      }

      if (!currentFile->open(currentDir, fileNameList[*currentIndex], O_RDONLY)) {
        return NextFrameStatus{SDStatus::error_file_no_open, reachedEndOfFolder};
      }

      if (currentFile->isDir()) {
        DirListStatus listStatus = listAllFiles(subFileNames, currentFile, &currentSubFileHolder, false);
        if (listStatus.status != SDStatus::success) {
          return NextFrameStatus{listStatus.status, reachedEndOfFolder};
        }

        subFileNamesLength = listStatus.amountOfFiles;
        currentSubFileNameIndex = -1;

        NextFrameStatus nextStatus = SDReader::selectNextFrame();
        if (nextStatus.status != SDStatus::success) {
          return NextFrameStatus{nextStatus.status, reachedEndOfFolder};
        }
      }

      // readBmp(currentFile);

      return NextFrameStatus{SDStatus::success, reachedEndOfFolder};
    }

    DirListStatus listAllFolders(uint16_t folderIndexList[], SdFile* rootDir, SdFile* currentFolder) {
      if (!rootDir->isOpen()) {
        return DirListStatus{SDStatus::error_root_closed, 0};
      }

      char nameBuffer[MaxFileNameLength] = {};
      bool isDirectory = false;

      FileCountUInt folderCount = 0;

      while (currentFolder->openNext(rootDir, O_RDONLY)) {
        folderIndexList[folderCount] = currentFolder->dirIndex();

        isDirectory = currentFolder->isDir();

        if (!currentFolder->getName(nameBuffer, MaxFileNameLength))
          return DirListStatus{SDStatus::error_folder_not_get_name, folderCount};
        if (!currentFolder->close())
          return DirListStatus{SDStatus::error_folder_no_close, folderCount};

        if (isHiddenChar(nameBuffer[0]) || strcmp(nameBuffer, "System Volum") == 0)
          continue;

        if (!isDirectory)
          return DirListStatus{SDStatus::error_non_folder_in_root, folderCount};

        if (folderCount == MaxFilesInFolder - 1)
          return DirListStatus{SDStatus::error_too_many_files, folderCount};

        folderCount++;
      }

      if (folderCount == 0) {
        return DirListStatus{SDStatus::error_no_files, folderCount};
      }

      rootDir->rewind();

      return DirListStatus{SDStatus::success, folderCount};
    }

    DirListStatus listAllFiles(char* fileNameList[], SdFile* currentDir, SdFile* currentFile, bool allowFolders) {
      if (!currentDir->isOpen()) {
        return DirListStatus{SDStatus::error_folder_closed, 0};
      }

      if (currentFile->isOpen()) {
        if (!currentFile->close()) {
          return DirListStatus{SDStatus::error_file_no_close, 0};
        }
      }

      bool isFile = false;

      FileCountUInt fileCount = 0;

      while (currentFile->openNext(currentDir, O_RDONLY)) {
        isFile = currentFile->isFile();

        if (!currentFile->getName(fileNameList[fileCount], MaxFileNameLength))
          return DirListStatus{SDStatus::error_file_no_get_name, fileCount};
        if (!currentFile->close())
          return DirListStatus{SDStatus::error_file_no_close, fileCount};

        if (isHiddenChar(fileNameList[fileCount][0]))
          continue;

        if (!allowFolders && !isFile)
          return DirListStatus{SDStatus::error_non_file_in_folder, fileCount};

        if (fileCount == MaxFilesInFolder - 1) {
          return DirListStatus{SDStatus::error_too_many_files, fileCount};
        }
        fileCount++;
      }

      if (fileCount == 0) {
        return DirListStatus{SDStatus::error_no_files, fileCount};
      }

      currentDir->rewind();

      sortArray(fileNameList, fileCount);

#ifdef DEBUG_MODE
      Serial.println("Sorted files:");
      for (FileCountUInt i = 0; i < fileCount; i++) {
        Serial.println(fileNameList[i]);
      }
#endif

      return DirListStatus{SDStatus::success, fileCount};
    }
  }  // namespace

  using namespace __internal;

  SdFileStatus getRootDirectory() {
    if (rootDirHolder.isOpen() && rootDirHolder.isDir()) {
      return SdFileStatus{SDStatus::success, &rootDirHolder};
    }
    return SdFileStatus{SDStatus::error_root_closed, 0};
  }

  SdFileStatus getCurrentFile() {
    if (currentSubFileHolder.isOpen() && currentSubFileHolder.isFile()) {
      SdFileStatus{SDStatus::success, &currentSubFileHolder};
    }
    if (currentFileHolder.isOpen() && currentFileHolder.isFile()) {
      SdFileStatus{SDStatus::success, &currentFileHolder};
    }
    return SdFileStatus{SDStatus::error_file_closed, 0};
  }

  SDStatus initialize() {
    initSDVariables();

    if (!sd.begin(SS, SD_SCK_MHZ(50))) {
      return SDStatus::error_init;
    }

    if (!rootDirHolder.open("/")) {
      return SDStatus::error_root_no_open;
    }

    rootDirHolder.rewind();

    DirListStatus listStatus = listAllFolders(folderIndexes, &rootDirHolder, &currentDirHolder);
    if (listStatus.status != SDStatus::success) {
      return listStatus.status;
    }
    folderIndexesLength = listStatus.amountOfFiles;
    currentFolderIndexesIndex = -1;

    return SDStatus::success;
  }

  SDStatus selectNextDirectory(bool useRandomOrder) {
    if (useRandomOrder) {
      FileCountUInt newIndex = rand() % folderIndexesLength;

      if (currentFolderIndexesIndex == newIndex + 1) {
        newIndex -= 1;
      }

      currentFolderIndexesIndex = newIndex;
    }
    return selectNextDirectory(folderIndexes, folderIndexesLength, &currentFolderIndexesIndex, &rootDirHolder, &currentDirHolder,
                               &currentFileHolder, &currentSubFileHolder);
  }

  NextFrameStatus selectNextFrame() {
    if (currentFileHolder.isDir()) {
      NextFrameStatus nextStatus =
          selectNextFrame(subFileNames, subFileNamesLength, &currentSubFileNameIndex, &currentFileHolder, &currentSubFileHolder, true);

      if (nextStatus.status != SDStatus::success) {
        return nextStatus;
      }

      if (nextStatus.hasReachedEnd) {
        return selectNextFrame(fileNames, fileNamesLength, &currentFileNameIndex, &currentDirHolder, &currentFileHolder, false);
      }

      return nextStatus;
    } else {
      return selectNextFrame(fileNames, fileNamesLength, &currentFileNameIndex, &currentDirHolder, &currentFileHolder, false);
    }
  }

}  // namespace SDReader
