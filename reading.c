#include "meta_ext.h"

bool	find_exif(FILE *file, t_data *data)
{
	size_t	bytesRead;

	if (!file)
		return (false);
	while ((bytesRead = fread(data->buffer, 1, sizeof(data->buffer), file)) > 0)
	{
		for (size_t i = 0; i < bytesRead - 10; i++)
		{
			if (data->buffer[i] == 0xFF && data->buffer[i + 1] == 0xE1 &&
				data->buffer[i + 4] == 0x45 && data->buffer[i + 5] == 0x78 &&
				data->buffer[i + 6] == 0x69 && data->buffer[i + 7] == 0x66 &&
				data->buffer[i + 8] == 0x00 && data->buffer[i + 9] == 0x00)
			{
				data->pos = ftell(file) - bytesRead + i;
				return (true);
			}
		}
	}
	return (false);
}

bool	read_file(FILE *file, t_data *data)
{
	size_t	bytesRead;

	if (find_exif(file, data))
	{
		printf("EXIF data found at position %zu\n", data->pos);
		// Move to EXIF position and load new buffer
		fseek(file, data->pos, SEEK_SET);
		bytesRead = fread(data->buffer, 1, sizeof(data->buffer), file);
		data->byt_read = bytesRead;
		if (!find_tiff(file, data, bytesRead))
		{
			printf("TIFF header not found\n");
			return (false);
		}
		printf("\nEXIF data extraction complete.\n");
		find_tags(file, data);
		return (true);
	}
	else
		printf("EXIF data not found\n");
	fclose(file);
	return (false);
}

bool	find_tiff(FILE *file, t_data *data, size_t bytread)
{
	for (size_t i = 0; i < bytread; i++)
	{
		if (data->buffer[i] == 0x49 && data->buffer[i + 1] == 0x49 &&data->buffer[i + 2] == 0x2A && data->buffer[i + 3] == 0x00)
		{
			printf("TIFF header found at buffer offset %zu: %02X %02X %02X %02X\n", i, data->buffer[i], data->buffer[i + 1],data->buffer[i + 2], data->buffer[i + 3]);
			data->byt_read = bytread;
			data->tiff_start = i;
			return (true);
		}
	}
	return (false);
}

/* type = kind of data
Count = how many items of that type.
Offset = where to find the data if it's too big to fit in the 4-byte space.*/
bool	find_tags(FILE *file, t_data *data)
{
	bool		any_found;
	size_t		tiff;
	uint32_t	ifd_offset;
	size_t		ifd_start;
	uint16_t	entry_count;
	size_t		entry_offset;
	uint16_t	tag;

	any_found = false;
	tiff = data->tiff_start;
	ifd_offset = data->buffer[tiff + 4] | (data->buffer[tiff+ 5] << 8) | (data->buffer[tiff + 6] << 16) | (data->buffer[tiff+ 7] << 24);
	ifd_start = tiff + ifd_offset;
	entry_count = data->buffer[ifd_start] | (data->buffer[ifd_start + 1] << 8);

	for (int i = 0; i < entry_count; i++)
	{
		entry_offset = ifd_start + 2 + (i * 12);
		tag = data->buffer[entry_offset] | (data->buffer[entry_offset + 1] << 8);
		if (tag == 0x0110)
		{ // Model tag
			printf("Found 'Model' tag at offset %zu\n", entry_offset);
			get_info(file, data, entry_offset, tag);
			data->tag = MODEL;
			make_tags(file, data, &data->res_data);
			any_found = true;
		}
		else if  (tag == 0x010F)
		{ // Make tag
			printf("Found 'Make' tag at offset %zu\n", entry_offset);
			get_info(file, data, entry_offset, tag);
            data->tag = MAKE;
            make_tags(file, data, &data->res_data);
			any_found = true;
		}
	}
	return (any_found);
}

void	get_info(FILE *file, t_data *data, int offset, uint16_t tag)
{
	data->type = data->buffer[offset + 2] | (data->buffer[offset + 3] << 8);
	data->count = data->buffer[offset + 4] | (data->buffer[offset+ 5] << 8) | (data->buffer[offset + 6] << 16) | (data->buffer[offset+ 7] << 24);
	data->offset = data->buffer[offset + 8] | (data->buffer[offset+ 9] << 8) | (data->buffer[offset+ 10] << 16) | (data->buffer[offset + 11] << 24);
	printf("get info Type: %u, Count: %u, Offset: %u\n", data->type, data->count,data->offset);
}

void	make_tags(FILE *file, t_data *data, t_res *res)
{
	long	current;
	size_t	absolute_offset;
	size_t	bytesRead;
	size_t	start_idx;

	if (data->type == 2 && data->count < 256)
        str_tags(file, data, res);
	return;
}

void	str_tags(FILE *file, t_data *data, t_res *res)
{
	long	current;
	size_t	absolute_offset;
	size_t	bytesRead;
	size_t	start_idx;

	bytesRead = 0;
	current = ftell(file);
	// Adjust the offset calculation to be relative to the TIFF start
	absolute_offset = data->offset + data->tiff_start;
    
	// Print debug information about offsets and counts
	printf("Offset: %u, TIFF Start: %u, Absolute Offset: %zu, Count: %u\n", data->offset, data->tiff_start, absolute_offset, data->count);
	
    // Seek to the absolute position of the string data
	fseek(file, absolute_offset, SEEK_SET);
    
	// Read string
	char str[256] = {0};
    bytesRead = fread(str, 1, data->count, file);
    str[data->count] = '\0'; 

    //debug
    // printf("Bytes read: %zu\n", bytesRead);
    // for (size_t i = 0; i < bytesRead; i++)
    //     printf("%02X ", (unsigned char)str[i]);
    // printf("\n");

	str[bytesRead] = '\0';
	// Trim null bytes if necessary
	start_idx = 2;
	while (start_idx < bytesRead && (str[start_idx] < 32 || str[start_idx] > 126))
		start_idx++;
	// Print out the cleaned string (skip the padding)
    if (start_idx < bytesRead) {
		// for(size_t i = 0; i < bytesRead; i++)
		// 	printf("%c, %zu\n", str[i], i);
        printf("\nTag: %i\n", data->tag);
        if (data->tag == MAKE)
            res->make = strdup(&str[start_idx]);
        else if (data->tag == MODEL)
            res->model = strdup(&str[start_idx]);
		// printf("start index: %zu\n", start_idx);
		// printf("bytes read: %zu\n", bytesRead);
		// printf("res model: %s\nres make:%s\n", res->model, res->make);
		// printf("this ended\n");
		fseek(file, current, SEEK_SET);
        return;
    }
    printf("No valid string data found.\n");
    fseek(file, current, SEEK_SET);
}
