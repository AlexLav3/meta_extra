#include "meta_ext.h"

int	main(void)
{
	t_data	*data;
	t_res	*res;
	FILE	*file;

	res = malloc(sizeof(t_res));
	if(!res)
		return (EXIT_FAILURE);
	data = malloc(sizeof(t_data));
	if(!data)
		return (EXIT_FAILURE);
	file = fopen("/home/elavrich/Documents/homegit/test2.jpg", "rb");
	read_file(file, data);
    print_res((&data->res_data));
    free(data);
    free(res);
	return (0);
}
