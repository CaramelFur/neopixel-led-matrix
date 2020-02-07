#include "bmp.h"

static uint32_t colorMap[256] = {};

static Better_NeoMatrix *matrix;

uint16_t invertY(uint16_t i)
{
  return displayHeight - 1 - i;
}

void insertPixel(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue)
{

  matrix->drawRGBPixel(pos % displayWidth, invertY(pos / displayWidth), red, green, blue);
}

void insertPixel(uint32_t pos, uint32_t rgb)
{
  uint8_t red = rgb >> 16;
  uint8_t green = rgb >> 8;
  uint8_t blue = rgb;

  return insertPixel(pos, red, green, blue);
}

// Insert a pixel from map into the pixelArray but first find the correct color
void insertMappedPixel(uint32_t pos, uint8_t color)
{
  return insertPixel(pos, colorMap[color]);
}

void readBmpData(SdFile *bmpFile, uint32_t image_offset, uint32_t pixels, uint16_t bits_per_pixel)
{
  if (!bmpFile->seekSet(image_offset))
  {
    halt("No data found in bmp at given offset: " + image_offset);
  }

  /*Serial.print("Reading with a bit-depth of: ");
  Serial.println(bits_per_pixel);*/

  for (uint32_t i = 0; i < pixels; i++)
  {
    if (bits_per_pixel == 8)
    {
      uint8_t buf;
      if (!bmpFile->read(&buf, 1))
        halt("Could not read from bmp");

      insertMappedPixel(i, buf);
    }
    else if (bits_per_pixel == 4)
    {
      uint8_t buf;
      if (!bmpFile->read(&buf, 1))
        halt("Could not read from bmp");

      insertMappedPixel(i, buf >> 4);
      buf = buf << 4;
      i++;
      insertMappedPixel(i, buf >> 4);
    }
    else if (bits_per_pixel == 24)
    {
      uint32_t buf;
      if (!bmpFile->read(&buf, 3))
        halt("Could not read from bmp");

      insertPixel(i, buf);

      // This makes sure it skips spacing
      if (i % displayWidth == displayWidth - 1 && (displayWidth * 3) % 4 != 0)
      {
        if (!bmpFile->read(&buf, (displayWidth * 3) % 4))
          halt("Could not read from bmp");
      }
    }
    else
    {
      halt("Got invalid bit depth while reading bmp: " + bits_per_pixel);
    }
  }
}

void readBmpColormap(SdFile *bmpFile, uint32_t amountOfColors)
{
  for (uint16_t i = 0; i < amountOfColors; i++)
  {
    if (!bmpFile->read(&colorMap[i], 4))
      halt("Could not read colormap from bmp");
  }
}

void readBmp(SdFile *bmpFile)
{
  if (!bmpFile->isOpen())
  {
    halt("Tried to read header but no file is open");
  }

  bmp_file_header_t fileHeader;
  bmpFile->read(&fileHeader, sizeof(fileHeader));

  if (fileHeader.signature != 0x4D42) // "BM"
  {
    halt("Encountered invalid bmp: " + getFileName(bmpFile));
  }

  uint32_t header_size;
  bmpFile->read(&header_size, sizeof(header_size));

  if (header_size != 40 && header_size != 52 && header_size != 56 && header_size != 108 && header_size != 124)
  {
    halt("Bmp header has unexpected size of " + String(header_size) + ", please use the NT header.");
  }

  bmp_image_header_t imageHeader;
  bmpFile->read(&imageHeader, sizeof(imageHeader));
  bmpFile->seekCur(header_size - sizeof(imageHeader) - sizeof(header_size));

  if (imageHeader.color_planes != 1)
    halt("Incorrect color plane in header: " + String(imageHeader.color_planes));

  if (imageHeader.compression_method != 0)
    halt("Please use uncompressed bmps");

  if (imageHeader.bits_per_pixel != 4 && imageHeader.bits_per_pixel != 8 && imageHeader.bits_per_pixel != 24)
    halt("Please use a bit depth of 4, 8 or 24, and not: " + String(imageHeader.bits_per_pixel));

  if (imageHeader.colors_in_palette > 256)
    halt("Please use a color pallete of 256 colors or less and not: " + String(imageHeader.colors_in_palette));

  if (imageHeader.image_width != displayWidth || imageHeader.image_height != displayHeight)
    halt("Image width and height are not correct, found " + String(imageHeader.image_width) + "x" + String(imageHeader.image_height));

  if (imageHeader.colors_in_palette > 0)
  {
    readBmpColormap(bmpFile, imageHeader.colors_in_palette);
  }

  readBmpData(bmpFile, fileHeader.image_offset, imageHeader.image_width * imageHeader.image_height, imageHeader.bits_per_pixel);
}

void InitBmp(Better_NeoMatrix *newMatrix)
{
  matrix = newMatrix;
}