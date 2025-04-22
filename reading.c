#include "meta_ext.h"

bool        find_exif(FILE *file, t_data *data)
{
    size_t bytesRead;
    
    if (!file) return false;
    // Read the file in chunks
    while ((bytesRead = fread(data->buffer, 1, sizeof(data->buffer), file)) > 0)
    {
        // Look for the EXIF signature
        for (size_t i = 0; i < bytesRead; i++)
        {
            if (data->buffer[i] == 0x45 && data->buffer[i + 1] == 0x78 && 
                data->buffer[i + 2] == 0x69 && data->buffer[i + 3] == 0x66 && 
                data->buffer[i + 4] == 0x00 && data->buffer[i + 5] == 0x00)
            {
                data->pos = i;
                return true; // Found EXIF
            }
        }
    }
    return false; // EXIF not found
}

void	read_file(FILE *file, t_data *data)
{
	unsigned char	buffer[2048];
	size_t			bytesRead;

	if (find_exif(file, data))
    {
        printf("EXIF data found at position %zu\n", data->pos);
        fseek(file, data->pos, SEEK_SET); // Go back to the position of EXIF data

        bool found_end_marker = false;
        while ((bytesRead = fread(data->buffer, 1, sizeof(data->buffer), file)) > 0 && !found_end_marker) {
            // Check for the end of EXIF data (JPEG end marker FF D9)
            for (size_t i = 0; i < bytesRead - 1; i++) {
                if (data->buffer[i] == 0xFF && data->buffer[i + 1] == 0xD9) {
                    found_end_marker = true;
                    bytesRead = i + 2;  // Stop after the FF D9 marker
                    break;
                }
            }
            // Print the EXIF data
            for (size_t i = 0; i < bytesRead; i += 2) {
                uint16_t val = buffer[i] | (buffer[i + 1] << 8); // Little endian
                printf("%04X ", val); // or use %5d for decimal
            }
        }
        printf("\nEXIF data extraction complete.\n");
    }
    else 
    {
        printf("EXIF data not found\n");
    }
    fclose(file);
}

