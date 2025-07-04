#include "meta_ext.h"

static void freeAll(t_data *data, t_res *res)
{
  if(res->model)
    free(res->model);
  if(res->make)
    free(res->make);
  if(data->buffer)
    free(data->buffer);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return (1);
  }
  t_data *data = malloc(sizeof(t_data));
  if (!data)
    return 0;
  memset(data, 0, sizeof(t_data));
  data->file_name = argv[1];
  data->file = fopen(data->file_name, "rb");
  if (data->file == NULL) {
    perror("Error opening file");
    free(data);
    return (1);
  }
  if (fseek(data->file, 0, SEEK_END) < 0) {
        fclose(data->file);
        return -1;
  }
  data->buff_size = ftell(data->file);
  if (data->buff_size < 0) {
    fclose(data->file);
    return -1;
  }
  rewind(data->file);
  data->buffer = malloc(data->buff_size);
  if (!data->buffer) {
    fclose(data->file);
    return -1;
  }
  //fread(data->buffer, 1, data->buff_size, data->file);
  read_file(data->file, data);
  fclose(data->file);
  print_res((&data->res_data));
  freeAll(data, &data->res_data);
  free(data);
  return (0);
}
