#include "meta_ext.h"

int	main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return (1);
	}
	t_data	*data = malloc(sizeof(t_data));
	if(!data)
		return 0; 
	data->file_name = argv[1];
	data->file = fopen(data->file_name, "rb");
	if (!data->file)
	{
		perror("Error opening file");
		free(data);
		return (1);
	}
	read_file(data->file, data);
    print_res((&data->res_data));

    free(data);
	return (0);
}
