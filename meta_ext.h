#ifndef META_EXT
# define META_EXT

# include <limits.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

#define MAKE 1
#define MODEL 2

typedef struct
{
	char			*model;
	char 			*make;
	char			settings[INT_MAX];
	long			date;
	float			time;
	char			loc[INT_MAX];
}					t_res;

typedef struct
{
	unsigned char	buffer[INT_MAX];
	size_t			byt_read;
	size_t			tiff_start;
	uint16_t		type;
	uint16_t		count;
	uint16_t		offset;
	size_t			pos;
	int 			tag;
	t_res			res_data;
}					t_data;

bool				read_file(FILE *file, t_data *data);
bool				find_exif(FILE *file, t_data *data);
bool				find_tiff(FILE *file, t_data *data, size_t bytread);
bool				find_tags(FILE *file, t_data *data);
void				get_info(FILE *file, t_data *data, int i, uint16_t tag);
void				make_tags(FILE *file, t_data *data, t_res *res);
void				str_tags(FILE *file, t_data *data, t_res *res);

void				print_res(t_res *res);

#endif