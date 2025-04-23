#include "meta_ext.h"

void	print_res(t_res *res)
{
	if (!res)
		return ;
	if (res->model)
		printf("Camera Make: %s\n", res->model);
}