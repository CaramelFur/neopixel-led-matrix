#include "error.h"

bool isSuccess(BmpReader::BMPStatus status) {
  using namespace BmpReader;

  switch (status) {
    case BMPStatus::success:
      return true;
    case BMPStatus::error_file_no_open:
      Halt::halt("Tried to read bmp from an unopened file");
    case BMPStatus::error_invalid_header_sig:
      Halt::halt("Encountered invalid bmp header signature");
    case BMPStatus::error_invalid_header_length:
      Halt::halt("Bmp has unsupported header length");
    case BMPStatus::error_unsup_color_plane:
      Halt::halt("Bmp has unsupported color plane");
    case BMPStatus::error_unsup_compression:
      Halt::halt("Bmp uses compression, this is unsupported");
    case BMPStatus::error_unsup_bits_pixel:
      Halt::halt("Bmp uses an unsupported pixel depth");
    case BMPStatus::error_unsup_color_palette:
      Halt::halt("Bmp uses more than supported pallete colors, 256 is the limit");
    case BMPStatus::error_incorrect_dimension:
      Halt::halt("Bmp has the incorrect dimensions for this matrix");
    case BMPStatus::error_read_colormap:
      Halt::halt("Unable to read the colormap from the bmp");
    case BMPStatus::error_noinit_matrix:
      Halt::halt("The bmp module has not been initialized");
    case BMPStatus::error_no_data_at_offset:
      Halt::halt("There was no pixeldata at the bmp supplied offset");
    case BMPStatus::error_file_no_read:
      Halt::halt("There was a problem with reading from the bmp file");
  }

  return false;
}

bool isSuccess(ConfigManager::CFGStatus status) {
  using namespace ConfigManager;

  switch (status) {
    case CFGStatus::success:
      return true;
    case CFGStatus::verbose_no_config_present:
      return true;
    case CFGStatus::error_invalid_config:
      Halt::halt("Encountered invalid config file");
    case CFGStatus::error_invalid_config_type:
      Halt::halt("An invalid config type was passed");
    case CFGStatus::error_config_no_open:
      Halt::halt("Could not open the config file");
    case CFGStatus::error_config_no_close:
      Halt::halt("Could not close the config file");
  }

  return false;
}

bool isSuccess(SDReader::SDStatus status) {
  using namespace SDReader;

  switch (status) {
    case SDStatus::success:
      return true;
    case SDStatus::error_init:
      Halt::halt("Could not open / initialize sd card");
    case SDStatus::error_root_no_open:
      Halt::halt("Could not open the root of the sd card");
    case SDStatus::error_folder_no_open:
      Halt::halt("Could not open folder");
    case SDStatus::error_file_no_open:
      Halt::halt("Could not open file");
    case SDStatus::error_root_closed:
      Halt::halt("Expected root directory to be open, but it was closed");
    case SDStatus::error_folder_closed:
      Halt::halt("Expected directory to be open, but it was closed");
    case SDStatus::error_file_closed:
      Halt::halt("Expected file to be open, but it was closed");
    case SDStatus::error_folder_not_get_name:
      Halt::halt("Could not get the name of folder");
    case SDStatus::error_folder_no_close:
      Halt::halt("Could not close folder");
    case SDStatus::error_file_no_get_name:
      Halt::halt("Could not get the name of file");
    case SDStatus::error_file_no_close:
      Halt::halt("Could not close file");
    case SDStatus::error_non_folder_in_root:
      Halt::halt("Found a non-folder file type in root, this is not good");
    case SDStatus::error_non_file_in_folder:
      Halt::halt("Found a non-file file type in a folder");
    case SDStatus::error_too_many_files:
      Halt::halt("There are too many files in a single directory for the esp to handle");
    case SDStatus::error_no_files:
      Halt::halt("There were no files in a directory");
  }

  return false;
}
