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

    size_t  pos; 
    unsigned char	buffer[2048];

}   t_data;

void		read_file(FILE *file, t_data *data);
bool        find_exif(FILE *file, t_data *data);

#endif