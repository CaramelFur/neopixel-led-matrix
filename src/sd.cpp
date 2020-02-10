#include "sd.h"

// Sd holders
static SdFat sd;

static SdFile rootDirHolder;
static SdFile currentDirHolder;
static SdFile currentSubFileHolder;
static SdFile currentFileHolder;

static FileCountUInt currentFolderIndexesIndex = 0;
static FileCountUInt folderIndexesLength = 0;
static uint16_t folderIndexes[MaxFolders] = {};

static FileCountUInt currentFileNameIndex = 0;
static FileCountUInt fileNamesLength = 0;
static char *fileNames[MaxFilesInFolder] = {};

static FileCountUInt currentSubFileNameIndex = 0;
static FileCountUInt subFileNamesLength = 0;
static char *subFileNames[MaxFilesInFolder] = {};

FileCountUInt listAllFiles(char *fileNameList[], SdFile *currentDir, SdFile *currentFile, bool allowFolders = true);

bool isHiddenChar(char toTest)
{
  for (uint8_t i = 0; i < hiddenCharsLen; i++)
  {
    if (hiddenChars[i] == toTest)
      return true;
  }
  return false;
}

bool selectNextFrame(char *fileNameList[], FileCountUInt fileNameListLength, FileCountUInt *currentIndex, SdFile *currentDir, SdFile *currentFile, bool stopOnLast = false)
{
  bool isEnd = false;
  if (currentFile->isOpen())
  {
    if (!currentFile->close())
    {
      halt("Could no close current file: " + getFileName(currentFile));
    }
  }

  Serial.print("Opening frame from: ");
  currentDir->printName();
  Serial.println();

  *currentIndex = *currentIndex + 1;
  if (*currentIndex == fileNameListLength)
  {
    *currentIndex = 0;
    isEnd = true;
    if (stopOnLast)
      return isEnd;
  }

  if (!currentFile->open(currentDir, fileNameList[*currentIndex], O_RDONLY))
  {
    halt("Could not open next frame in dir: " + getFileName(currentDir));
  }

  if (currentFile->isDir())
  {
    subFileNamesLength = listAllFiles(subFileNames, currentFile, &currentSubFileHolder, false);
    Serial.println("Listed subfiles");
    currentSubFileNameIndex = -1;
    selectNextFrame();
    return isEnd;
  }

  readBmp(currentFile);
  return isEnd;
}

bool selectNextFrame()
{
  if (currentFileHolder.isDir())
  {
    
    if (selectNextFrame(subFileNames, subFileNamesLength, &currentSubFileNameIndex, &currentFileHolder, &currentSubFileHolder, true))
    {
      Serial.println("reached end");
      return selectNextFrame(fileNames, fileNamesLength, &currentFileNameIndex, &currentDirHolder, &currentFileHolder);
    }
  }
  else
  {
    return selectNextFrame(fileNames, fileNamesLength, &currentFileNameIndex, &currentDirHolder, &currentFileHolder);
  }
  return false;
}

FileCountUInt listAllFiles(char *fileNameList[], SdFile *currentDir, SdFile *currentFile, bool allowFolders)
{
  Serial.println("Listing subfiles");
  if (!currentDir->isOpen())
    halt("Tried to read frame but no directory is open");

  if (currentFile->isOpen())
  {
    if (!currentFile->close())
    {
      halt("Could no close current file: " + getFileName(currentFile));
    }
  }

  bool isFileBuffer = false;

  FileCountUInt fileCount = 0;
  while (currentFile->openNext(currentDir, O_RDONLY))
  {
    isFileBuffer = currentFile->isFile();
    if (!currentFile->getName(fileNameList[fileCount], MaxFileNameLength))
      halt("Could not get file name");
    if (!currentFile->close())
      halt("Could not close file while listing");

    if (isHiddenChar(fileNameList[fileCount][0]))
      continue;
      
    if (!allowFolders && !isFileBuffer)
      halt("Please only store images in the folder: " + getFileName(currentDir));

    if (fileCount == MaxFilesInFolder - 1)
    {
      halt("There are too many frames inside the folder: " + getFileName(currentDir));
    }
    fileCount++;
  }

  if (fileCount == 0)
  {
    halt("There are no frames inside the folder: " + getFileName(currentDir));
  }

  currentDir->rewind();

  sortArray(fileNameList, fileCount);

  for (FileCountUInt i = 0; i < fileCount; i++)
  {
    Serial.println(fileNameList[i]);
  }

  return fileCount;
}

void selectNextDirectory(uint16 folderIndexList[], FileCountUInt folderIndexListLength, FileCountUInt *currentIndexListIndex, SdFile *rootDir, SdFile *currentDir, SdFile *currentFile, SdFile *currentSubFile)
{
  if (currentDir->isOpen())
  {
    if (!currentDir->close())
    {
      halt("Could not close current dir: " + getFileName(currentDir));
    }
  }

  if (currentFile->isOpen())
  {
    if (!currentFile->close())
    {
      halt("Could not close current file: " + getFileName(currentFile));
    }
  }

  if (currentSubFile->isOpen())
  {
    if (!currentSubFile->close())
    {
      halt("Could not close current subfile: " + getFileName(currentSubFile));
    }
  }

  *currentIndexListIndex = *currentIndexListIndex + 1;
  if (*currentIndexListIndex == folderIndexListLength)
    *currentIndexListIndex = 0;

  if (!currentDir->open(rootDir, folderIndexList[*currentIndexListIndex], O_RDONLY))
  {
    halt("Could not open next directory in root");
  }

  Serial.print("Current directory: ");
  currentDir->printName();
  Serial.println();

  fileNamesLength = listAllFiles(fileNames, currentDir, currentFile);
  currentFileNameIndex = -1;

  if (!readConfigFile(currentDir, ConfigType::animation))
  {
    Serial.println("Couldn't find config file, using defaults");
  }

  selectNextFrame();
}

FileCountUInt listAllFolders(uint16_t folderIndexList[], SdFile *rootDir, SdFile *currentFolder)
{
  if (!rootDir->isOpen())
    halt("Tried to folder from unopened root");

  if (currentFolder->isOpen())
  {
    if (!currentFolder->close())
    {
      halt("Could not close current folder: " + getFileName(currentFolder));
    }
  }

  char nameBuffer[MaxFileNameLength] = {};
  bool isDirBuffer = false;

  FileCountUInt folderCount = 0;
  while (currentFolder->openNext(rootDir, O_RDONLY))
  {
    folderIndexList[folderCount] = currentFolder->dirIndex();
    isDirBuffer = currentFolder->isDir();
    if (!currentFolder->getName(nameBuffer, MaxFileNameLength))
      halt("Could not get file name of folder");
    if (!currentFolder->close())
      halt("Could not close folder while listing");

    if (isHiddenChar(nameBuffer[0]) || strcmp(nameBuffer, "System Volum") == 0)
      continue;

    if (!isDirBuffer)
      halt("Please only store folders in the root");

    if (folderCount == MaxFilesInFolder - 1)
    {
      halt("There are too many folders in root");
    }
    folderCount++;
  }

  if (folderCount == 0)
  {
    halt("There are no folders in root");
  }

  rootDir->rewind();

  return folderCount;
}

void selectNextDirectory(bool doRandom = false)
{
  if (doRandom)
  {
    FileCountUInt newIndex = rand() % folderIndexesLength;

    if (currentFolderIndexesIndex == newIndex + 1)
    {
      newIndex -= 1;
    }

    currentFolderIndexesIndex = newIndex;
  }
  return selectNextDirectory(folderIndexes, folderIndexesLength, &currentFolderIndexesIndex, &rootDirHolder, &currentDirHolder, &currentFileHolder, &currentSubFileHolder);
}

void initSDVariables()
{
  FileCountUInt i;
  for (i = 0; i < MaxFilesInFolder; i++)
  {
    fileNames[i] = new char[MaxFileNameLength];
  }
  for (i = 0; i < MaxFilesInFolder; i++)
  {
    subFileNames[i] = new char[MaxFileNameLength];
  }
}

void initSD()
{
  initSDVariables();

  while (!sd.begin(SS, SD_SCK_MHZ(50)))
  {
    halt("Could not initialize the sd", true);
  }

  if (!rootDirHolder.open("/"))
  {
    halt("Could not open the sd root");
  }

  if (!readConfigFile(&rootDirHolder, ConfigType::main))
  {
    Serial.println("Could not find main config file, using defaults");
  }

  rootDirHolder.rewind();

  folderIndexesLength = listAllFolders(folderIndexes, &rootDirHolder, &currentDirHolder);
  currentFolderIndexesIndex = -1;

  selectNextDirectory();
}
