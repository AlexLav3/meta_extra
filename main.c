#include "meta_ext.h"

int	main(void)
{
	t_data	*data;
	t_res	*res;
	FILE	*file;

	res = malloc(sizeof(t_res));
	data = malloc(sizeof(t_data));
	file = fopen("JPG file here", "rb");
	read_file(file, data);
    print_res((&data->res_data));
    free(data);
    free(res);
	return (0);
}
