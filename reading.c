#include "meta_ext.h"

bool find_exif(FILE *file, t_data *data) {
    size_t bytesRead;

    if (!file) return false;

    while ((bytesRead = fread(data->buffer, 1, sizeof(data->buffer), file)) > 0) {
        for (size_t i = 0; i < bytesRead - 10; i++) {
            if (data->buffer[i] == 0xFF && data->buffer[i + 1] == 0xE1 &&
                data->buffer[i + 4] == 0x45 && data->buffer[i + 5] == 0x78 &&
                data->buffer[i + 6] == 0x69 && data->buffer[i + 7] == 0x66 &&
                data->buffer[i + 8] == 0x00 && data->buffer[i + 9] == 0x00) {
                
                data->pos = ftell(file) - bytesRead + i;
                return true;
            }
        }
    }
    return false;
}

void read_file(FILE *file, t_data *data) {
    size_t bytesRead;

    if (find_exif(file, data)) {
        printf("EXIF data found at position %zu\n", data->pos);
        printf("\nEXIF data extraction complete.\n");
        find_tiff(file, data, bytesRead);
            
    } else {
        printf("EXIF data not found\n");
    }
    fclose(file);
}

bool find_tiff(FILE *file, t_data *data, size_t bytread)
{
    for (size_t i = 0; i < bytread - 3; i++) {
        if (data->buffer[i] == 0x49 && data->buffer[i + 1] == 0x49 &&
            data->buffer[i + 2] == 0x2A && data->buffer[i + 3] == 0x00) {
            printf("TIFF header found at buffer offset %zu: %02X %02X %02X %02X\n", i,
                   data->buffer[i], data->buffer[i+1], data->buffer[i+2], data->buffer[i+3]);
            return true;
        }
    }
    return false;
}