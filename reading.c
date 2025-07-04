#include "meta_ext.h"

bool find_exif(FILE *file, t_data *data) {
  size_t bytesRead = 0;

  if (!file)
    return (false);
  while ((bytesRead = fread(data->buffer, 1, data->buff_size, file)) > 0) {
    for (size_t i = 0; i < bytesRead - 10; i++) {
      if (data->buffer[i] == 0xFF && data->buffer[i + 1] == 0xE1 &&
          data->buffer[i + 4] == 0x45 && data->buffer[i + 5] == 0x78 &&
          data->buffer[i + 6] == 0x69 && data->buffer[i + 7] == 0x66 &&
          data->buffer[i + 8] == 0x00 && data->buffer[i + 9] == 0x00) {
        data->pos = ftell(file) - bytesRead + i;
        return (true);
      }
    }
  }
  return (false);
}

bool read_file(FILE *file, t_data *data) {
  size_t bytesRead = 0;

  if (find_exif(file, data)) {
    //printf("EXIF data found at position %zu\n", data->pos);
    fseek(file, data->pos, SEEK_SET); // Move to EXIF position and load new buffer
    bytesRead = fread(data->buffer, 1, data->buff_size, file);
    data->byt_read = bytesRead;
    if (!find_tiff(data, bytesRead)) {
       //printf("TIFF header not found\n");
      return (false);
    }
    //printf("\nEXIF data extraction complete.\n");
    find_tags(data);
    return (true);
  } 
  else
    printf("EXIF data not found\n");
  fclose(file);
  return (false);
}

bool find_tiff(t_data *data, size_t bytread) {
  for (size_t i = 0; i < bytread; i++) {
    if (data->buffer[i] == 0x49 && data->buffer[i + 1] == 0x49 &&
        data->buffer[i + 2] == 0x2A && data->buffer[i + 3] == 0x00) {
      // printf("TIFF header found at buffer offset %zu: %02X %02X %02X %02X\n",
      // i, data->buffer[i], data->buffer[i + 1],data->buffer[i + 2],
      // data->buffer[i + 3]);
      data->byt_read = bytread;
      data->tiff_start = i;
      return (true);
    }
  }
  return (false);
}

/* type = kind of data
Count = how many items of that type.
Offset = where to find the data if it's too big to fit in the 4-byte space.
*/
bool find_tags(t_data *data) {
  bool any = false;
  size_t entry_offset = 0;
  uint16_t tag = 0;
  size_t tiff = data->tiff_start;
  uint32_t ifd_offset = data->buffer[tiff + 4] | (data->buffer[tiff + 5] << 8) |
                        (data->buffer[tiff + 6] << 16) |
                        (data->buffer[tiff + 7] << 24);
  size_t ifd_start = tiff + ifd_offset;
  uint16_t entry_count =
      data->buffer[ifd_start] | (data->buffer[ifd_start + 1] << 8);

  for (int i = 0; i < entry_count; i++) {
    entry_offset = ifd_start + 2 + (i * 12);
    tag = data->buffer[entry_offset] | (data->buffer[entry_offset + 1] << 8);
    if (tag_found(tag, data)) {
      get_info(data, entry_offset, tag);
      make_tags(data->file, data, &data->res_data);
      any = true;
    }
  }
  for (int i = 0; i < entry_count; i++) {
    entry_offset = ifd_start + 2 + (i * 12);
    tag = data->buffer[entry_offset] | (data->buffer[entry_offset + 1] << 8);
    if (tag == 0x8825)
      find_gpt_tags(data->file, data, entry_count, ifd_start, entry_offset);
  }
  return any;
}

bool find_gpt_tags(FILE *file, t_data *data, uint16_t entry_count, size_t ifd_start, size_t entry_offset) {
  bool any = false;
  uint32_t gps_offset = data->buffer[entry_offset + 8] | (data->buffer[entry_offset + 9] << 8) |(data->buffer[entry_offset + 10] << 16) |(data->buffer[entry_offset + 11] << 24);
  size_t gps_ifd_start = data->tiff_start + gps_offset;
  uint16_t gps_entry_count = data->buffer[gps_ifd_start] | (data->buffer[gps_ifd_start + 1] << 8);

  for (int j = 0; j < gps_entry_count; j++) {
    size_t gps_entry_offset = gps_ifd_start + 2 + (j * 12);
    uint16_t gps_tag = data->buffer[gps_entry_offset] |(data->buffer[gps_entry_offset + 1] << 8);
    if (tag_found(gps_tag, data)) {
      get_info(data, gps_entry_offset, gps_tag);
      make_tags(file, data, &data->res_data);
      any = true;
    }
  }
  return any;
}

void get_info(t_data *data, int offset, uint16_t tag) {
   //printf("entering get info\n");
  if (data->tag == 2 || data->tag == 1 || data->tag == 3 || data->tag == 4)
    data->type = data->buffer[offset + 2] | (data->buffer[offset + 3] << 8);
  	data->count = data->buffer[offset + 4] | (data->buffer[offset + 5] << 8) | (data->buffer[offset + 6] << 16) |(data->buffer[offset + 7] << 24);
  	data->offset = data->buffer[offset + 8] | (data->buffer[offset + 9] << 8) |(data->buffer[offset + 10] << 16) | (data->buffer[offset + 11] << 24);
   //printf("get info Type: %u, Count: %u, Offset: %u\n", data->type,data->count,data->offset);
}

bool tag_found(uint16_t tag, t_data *data) {
   //printf("tag found TAG 0x%04X\n", tag);
  switch (tag) {
  case 0x0110: // Model
    // printf("Found 'Model' tag \n");
    data->tag = MODEL;
    return true;

  case 0x010F: // Make
    // printf("Found 'Make' tag \n");
    data->tag = MAKE;
    return true;

  case 0x0001: // North/South
     //printf("Found 'North/South' tag \n");
    data->tag = NOTHSOUTH;
    return true;

  case 0x0002: // Latitude
     //printf("Found 'Latitude' tag \n");
    data->type = 5;
    data->tag = LATITUDE;
    return true;

  case 0x0003: // East/West
     //printf("Found 'East/West' tag \n");
    data->tag = EASTWEAST;
    return true;

  case 0x0004: // Longitude
     //printf("Found 'Longitude' tag \n");
    data->type = 5;
    data->tag = LONGITUDE;
    return true;
  default:
    return false;
  }
}
