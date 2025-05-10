#include "meta_ext.h"

void make_tags(FILE *file, t_data *data, t_res *res) {
  // printf("entering MAKE TAGS\n");
  if (data->type == 2 && data->count < 256)
    str_tags(file, data, res);
  else if (data->type == 5 && (data->tag == LATITUDE || data->tag == LONGITUDE))
    rational_tags(file, data, res);
  // printf("Exit MAKE TAGS\n");
}

void str_tags(FILE *file, t_data *data, t_res *res) {
  long current = ftell(file);
  size_t absolute_offset = data->offset + data->tiff_start; // Adjust the offset calculation to be relative to the TIFF start
  size_t bytesRead = 0;

  // Print debug information about offsets and counts
  // printf("Offset: %u, TIFF Start: %u, Absolute Offset: %zu, Count: %u\n",
  // data->offset, data->tiff_start, absolute_offset, data->count);

  // Seek to the absolute position of the string data
  fseek(file, absolute_offset, SEEK_SET);

  // Read string
  char str[256] = {0};
  bytesRead = fread(str, 1, data->count, file);
  str[bytesRead] = '\0';

  // Trim null bytes if necessary
  size_t start_idx;
  while (start_idx < bytesRead && (str[start_idx] < 32 || str[start_idx] > 126))
    start_idx++;
  // get the cleaned string (skip the padding)
  if (start_idx < bytesRead) {
    // for(size_t i = 0; i < bytesRead; i++)
    // 	printf("%c, %zu\n", str[i], i);
    // printf("\nTag: %i\n", data->tag);
    if (data->tag == MAKE)
      res->make = strdup(&str[start_idx]);
    else if (data->tag == MODEL)
      res->model = strdup(&str[start_idx]);
    else if (data->tag == NOTHSOUTH)
      res->north_south = str[start_idx];
    else if (data->tag == EASTWEAST)
      res->east_west = str[start_idx];
    // debug info
    // printf("start index: %zu\n", start_idx);
    // printf("bytes read: %zu\n", bytesRead);
    // printf("res model: %s\nres make:%s\n", res->model, res->make);
    fseek(file, current, SEEK_SET);
    return;
  }
  // printf("No valid string data found.\n");
  fseek(file, current, SEEK_SET);
}

void rational_tags(FILE *file, t_data *data, t_res *res) {
  // printf("\nRational Tags: %i\n", data->tag);
  if (data->tag != LONGITUDE && data->tag != LATITUDE)
    return;

  size_t absolute_offset = data->offset + data->tiff_start;
  fseek(file, absolute_offset, SEEK_SET);

  Rational coords[3];
  for (int i = 0; i < 3; i++) {
    fread(&coords[i].numerator, 4, 1, file);
    fread(&coords[i].denominator, 4, 1, file);
  }

  GPS_Coord result = convert_coord(coords);

  if (data->tag == LONGITUDE)
    res->longitude = result;
  else if (data->tag == LATITUDE)
    res->latitude = result;
  else
    printf("no GPS coordinates found\n");
}
