#include "meta_ext.h"

bool	find_exif(FILE *file, t_data *data)
{
	size_t	bytesRead = 0;

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
	size_t	bytesRead = 0;

	if (find_exif(file, data))
	{
		printf("EXIF data found at position %zu\n", data->pos); 
		
		fseek(file, data->pos, SEEK_SET); // Move to EXIF position and load new buffer
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
	bool 		any = false;
	size_t		entry_offset = 0;
	uint16_t	tag = 0;
	size_t		tiff = data->tiff_start;
	uint32_t	ifd_offset = data->buffer[tiff + 4] | (data->buffer[tiff+ 5] << 8) | (data->buffer[tiff + 6] << 16) | (data->buffer[tiff+ 7] << 24);
	size_t		ifd_start = tiff + ifd_offset;
	uint16_t	entry_count = data->buffer[ifd_start] | (data->buffer[ifd_start + 1] << 8);
	
	for (int i = 0; i < entry_count; i++)
	{
		entry_offset = ifd_start + 2 + (i * 12);
		tag = data->buffer[entry_offset] | (data->buffer[entry_offset + 1] << 8);
		if (tag_found(tag, data))
		{
			get_info(file, data, entry_offset, tag);
			make_tags(file, data, &data->res_data);
			any = true;
		}
	}
	return any;
}

void	get_info(FILE *file, t_data *data, int offset, uint16_t tag)
{
	data->type = data->buffer[offset + 2] | (data->buffer[offset + 3] << 8);
	data->count = data->buffer[offset + 4] | (data->buffer[offset+ 5] << 8) | (data->buffer[offset + 6] << 16) | (data->buffer[offset+ 7] << 24);
	data->offset = data->buffer[offset + 8] | (data->buffer[offset+ 9] << 8) | (data->buffer[offset+ 10] << 16) | (data->buffer[offset + 11] << 24);
	printf("get info Type: %u, Count: %u, Offset: %u\n", data->type, data->count,data->offset);
}

bool tag_found(uint16_t tag, t_data *data)
{	
	switch (tag)
	{
		case 0x0110: // Model
			printf("Found 'Model' tag \n");
			data->tag = MODEL;
			return true;

		case 0x010F: // Make
			printf("Found 'Make' tag \n");
			data->tag = MAKE;
			return true;

		case 0x0001: // North/South
			printf("Found 'Make' tag \n");
			data->tag = NOTHSOUTH;
			return true;

		case 0x0002: // Latitude
			printf("Found 'Latitude' tag \n");
			data->tag = LATITUDE;
			return true;

		case 0x0003: // East/West
			printf("Found 'East/West' tag \n");
			data->tag = EASTWEAST;
			return true;

		case 0x0004: // Longitude
			printf("Found 'Longitude' tag \n");
			data->tag = LONGITUDE;
			return true;
		default:
			return false;
	}
}
