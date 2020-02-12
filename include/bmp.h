#include "Arduino.h"
#include "SdFat.h"
#include "allSettings.h"
#include "filefuncs.h"
#include "halt.h"
#include "matrix.h"

#pragma once

struct bmp_file_header_t {
  uint16_t signature;
  uint32_t file_size;
  uint16_t reserved[2];
  uint32_t image_offset;
} __attribute__((packed));

struct bmp_image_header_t {
  uint32_t image_width;
  uint32_t image_height;
  uint16_t color_planes;
  uint16_t bits_per_pixel;
  uint32_t compression_method;
  uint32_t image_size;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  uint32_t colors_in_palette;
  uint32_t important_colors;
} __attribute__((packed));

void readBmp(SdFile* bmpFile);

void InitBmp(Better_NeoMatrix* newMatrix);