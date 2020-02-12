#include "Arduino.h"
#include "SdFat.h"
#include "allSettings.h"
#include "filefuncs.h"
#include "matrix.h"

#pragma once

namespace BmpReader {
  enum BMPStatus {
    success,
    error_file_no_open,           // Tried to read from an unopened file
    error_invalid_header_sig,     // Encountered invalid header signatureq
    error_invalid_header_length,  // Bmp has unsupported header length
    error_unsup_color_plane,      // Bmp has unsupported color plane
    error_unsup_compression,      // Bmp uses compression, this is unsupported
    error_unsup_bits_pixel,       // Bmp uses an unsupported pixel depth
    error_unsup_color_palette,    // Bmp uses more than supported pallete colors
    error_incorrect_dimension,    // Bmp has the incorrect size for this matrix
    error_read_colormap,          // Unable to read the colormap from the bmp
    error_noinit_matrix,     // The pixel write function has not been initialized
    error_no_data_at_offset,      // There was no pixeldata at the bmp supplied offset
    error_file_no_read,           // There was a problem with reading from the image file
  };

  namespace __internal {
    // These are the header structure
    // These use __attribute__((packed)) because this disables the padding
    // Without the padding you can easily read data into the struct
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

    BMPStatus readBmpColormap(SdFile* bmpFile, uint32_t amountOfColors);

    BMPStatus readBmpData(SdFile* bmpFile, uint32_t image_offset, uint32_t pixels, uint16_t bits_per_pixel);

  }  // namespace __internal

  namespace pixel {
    uint16_t invertY(uint16_t i);

    void insert(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue);

    void insert(uint32_t pos, uint32_t rgb);

    void insertFromMap(uint32_t pos, uint8_t color);
  }  // namespace pixel

  void Initialize(Better_NeoMatrix* neoMatrix);

  BMPStatus readBmp(SdFile* bmpFile);
}  // namespace BmpReader
