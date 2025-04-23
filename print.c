#include "meta_ext.h"

void	print_res(t_res *res)
{
	if (!res)
		return ;
	printf("Camera Make: %s\n", res->make);
	printf("Camera Model: %s\n", res->model);
}