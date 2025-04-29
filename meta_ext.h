#ifndef META_EXT
# define META_EXT

# include <limits.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define MAKE 1
# define MODEL 2
# define NOTHSOUTH 3
# define EASTWEAST 4
# define LATITUDE 5
# define LONGITUDE 6

typedef struct
{
	uint32_t		numerator;
	uint32_t		denominator;
}					Rational;

typedef struct
{
	Rational		degrees;
	Rational		minutes;
	Rational		seconds;
}					GPS_Coord; //EXIF type 5

typedef struct
{
	//camera info EXIF type = 2, ASCII
	char			*model;
	char			*make;

	//location
	//EXIF type = 2, ASCII
	char 			north_south; // 'N or 'S'
	char			east_west; // 'E' or 'W'

	GPS_Coord		latitude; 
	GPS_Coord		longitude; 
	char			loc[INT_MAX]; //store end location result
}					t_res;

typedef struct
{
	unsigned char	buffer[INT_MAX];
	FILE			*file;
	char			*file_name;
	size_t			byt_read;
	size_t			tiff_start;
	uint16_t		type;
	uint16_t		count;
	uint16_t		offset;
	size_t			pos;
	int				tag;
	t_res			res_data;
}					t_data;

//reading and finding
bool				read_file(FILE *file, t_data *data);
bool				find_exif(FILE *file, t_data *data);
bool				find_tiff(FILE *file, t_data *data, size_t bytread);
bool				find_tags(FILE *file, t_data *data);
bool				find_gpt_tags(FILE *file, t_data *data, uint16_t entry_count, size_t ifd_start, size_t entry_offset);


//get information from the tags
void				get_info(FILE *file, t_data *data, int i, uint16_t tag);

//tag and result
bool 				tag_found(uint16_t tag, t_data *data);
void				make_tags(FILE *file, t_data *data, t_res *res);
void				str_tags(FILE *file, t_data *data, t_res *res);
void    			rational_tags(FILE *file, t_data *data, t_res *res);

//location 
GPS_Coord 			convert_coord(Rational *coords);
double 				convert_to_decimal(Rational *coords, char ref);
double 				rational_to_double(Rational number);
//result
void				print_res(t_res *res);

#endif