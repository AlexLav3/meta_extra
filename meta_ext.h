#ifndef META_EXT
# define META_EXT

# include <limits.h>
# include <stdint.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
#include <stdbool.h> 

typedef struct {

	char	model[INT_MAX];
	char	settings[INT_MAX];
	long	date;
	float	time;
	char	loc[INT_MAX];
}			t_res;

typedef struct {

    unsigned char	buffer[INT_MAX];
    size_t  pos; 

}   t_data;

void		read_file(FILE *file, t_data *data);
bool        find_exif(FILE *file, t_data *data);
bool 		find_tiff(FILE *file, t_data *data, size_t bytread);

#endif