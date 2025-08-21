#include "meta_ext.h"

void make_tags(FILE *file, t_data *data, t_res *res) {
  if (data->type == 2 && data->count < 256)
    str_tags(file, data, res);
  else if (data->type == 5 && (data->tag == LATITUDE || data->tag == LONGITUDE))
    rational_tags(file, data, res);
}

void str_tags(FILE *file, t_data *data, t_res *res) {
  long current = ftell(file);
  size_t absolute_offset = data->offset + data->tiff_start; // Adjust the offset calculation to be relative to the TIFF start
  size_t bytesRead = 0;

  // Seek to the absolute position of the string data
  fseek(file, absolute_offset, SEEK_SET);

  // Read string
  char str[256] = {0};
  bytesRead = fread(str, 1, data->count, file);
  str[bytesRead] = '\0';

  // Trim null bytes if necessary
  size_t start_idx = 2;
  while (start_idx < bytesRead && (str[start_idx] < 32 || str[start_idx] > 126))
    start_idx++;
  // get the cleaned string (skip the padding)
  if (start_idx < bytesRead) {
    if (data->tag == MAKE){
      if(res->make)
        free(res->make);
      res->make = strdup(&str[start_idx]);
    }
    else if (data->tag == MODEL){
      if(res->model)
        free(res->model);
      res->model = strdup(&str[start_idx]);
    }
    else if (data->tag == NOTHSOUTH){
      res->north_south = str[start_idx];
    }
    else if (data->tag == EASTWEAST){
      res->east_west = str[start_idx];
    }
    fseek(file, current, SEEK_SET);
    return;
  }
  fseek(file, current, SEEK_SET);
}

void rational_tags(FILE *file, t_data *data, t_res *res) {
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
