#include "bmp.h"

namespace BmpReader {
  namespace __internal {
    uint32_t colorMap[256] = {};

    void (*pixelWriteFunction)(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b) = 0;

    BMPStatus readBmpColormap(SdFile* bmpFile, uint32_t amountOfColors) {
      for (uint16_t i = 0; i < amountOfColors; i++) {
        if (!bmpFile->read(&colorMap[i], 4))
          return BMPStatus::error_read_colormap;
      }
      return BMPStatus::success;
    }

    BMPStatus readBmpData(SdFile* bmpFile, uint32_t image_offset, uint32_t pixels, uint16_t bits_per_pixel) {
      if (!bmpFile->seekSet(image_offset)) {
        return BMPStatus::error_no_data_at_offset;
      }

      for (uint32_t i = 0; i < pixels; i++) {
        if (bits_per_pixel == 8) {
          uint8_t buf;
          if (!bmpFile->read(&buf, 1))
            return BMPStatus::error_file_no_read;

          pixel::insertFromMap(i, buf);
        } else if (bits_per_pixel == 4) {
          uint8_t buf;
          if (!bmpFile->read(&buf, 1))
            return BMPStatus::error_file_no_read;

          pixel::insertFromMap(i, buf >> 4);
          buf = buf << 4;
          i++;
          pixel::insertFromMap(i, buf >> 4);
        } else if (bits_per_pixel == 24) {
          uint32_t buf;
          if (!bmpFile->read(&buf, 3))
            return BMPStatus::error_file_no_read;

          pixel::insert(i, buf);

          // This makes sure it skips spacing
          if (i % displayWidth == displayWidth - 1 && (displayWidth * 3) % 4 != 0) {
            if (!bmpFile->read(&buf, (displayWidth * 3) % 4))
              return BMPStatus::error_file_no_read;
          }
        } else {
          return BMPStatus::error_unsup_bits_pixel;
        }
      }
      return BMPStatus::success;
    }

  }  // namespace __internal

  using namespace __internal;

  namespace pixel {
    uint16_t invertY(uint16_t i) { return displayHeight - 1 - i; }

    void insert(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue) {
      pixelWriteFunction(pos % displayWidth, invertY(pos / displayWidth), red, green, blue);
    }

    void insert(uint32_t pos, uint32_t rgb) {
      uint8_t red = rgb >> 16;
      uint8_t green = rgb >> 8;
      uint8_t blue = rgb;

      return insert(pos, red, green, blue);
    }

    // Insert a pixel from map into the pixelArray but first find the correct color
    void insertFromMap(uint32_t pos, uint8_t color) { return insert(pos, colorMap[color]); }
  }  // namespace pixel

  void Initialize(void (*writePixel)(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b)) { pixelWriteFunction = writePixel; }

  BMPStatus readBmp(SdFile* bmpFile) {
    if (!bmpFile->isOpen()) {
      return BMPStatus::error_file_no_open;
    }

    if (pixelWriteFunction == 0) {
      return BMPStatus::error_noinit_pixel_write;
    }

    bmp_file_header_t fileHeader;
    bmpFile->read(&fileHeader, sizeof(fileHeader));

    if (fileHeader.signature != 0x4D42)  // "BM"
    {
      return BMPStatus::error_invalid_header_sig;
    }

    uint32_t header_size;
    bmpFile->read(&header_size, sizeof(header_size));

    if (header_size != 40 && header_size != 52 && header_size != 56 && header_size != 108 && header_size != 124) {
      return BMPStatus::error_invalid_header_length;
    }

    bmp_image_header_t imageHeader;
    bmpFile->read(&imageHeader, sizeof(imageHeader));
    bmpFile->seekCur(header_size - sizeof(imageHeader) - sizeof(header_size));

    if (imageHeader.color_planes != 1)
      return BMPStatus::error_unsup_color_plane;

    if (imageHeader.compression_method != 0)
      return BMPStatus::error_unsup_compression;

    if (imageHeader.bits_per_pixel != 4 && imageHeader.bits_per_pixel != 8 && imageHeader.bits_per_pixel != 24)
      return BMPStatus::error_unsup_bits_pixel;

    if (imageHeader.colors_in_palette > 256)
      return BMPStatus::error_unsup_color_palette;

    if (imageHeader.image_width != displayWidth || imageHeader.image_height != displayHeight)
      return BMPStatus::error_incorrect_dimension;

    if (imageHeader.colors_in_palette > 0) {
      readBmpColormap(bmpFile, imageHeader.colors_in_palette);
    }

    readBmpData(bmpFile, fileHeader.image_offset, imageHeader.image_width * imageHeader.image_height, imageHeader.bits_per_pixel);

    return BMPStatus::success;
  }

}  // namespace BmpReader
