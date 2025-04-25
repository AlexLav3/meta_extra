#include "meta_ext.h"

void	print_res(t_res *res)
{
	if (!res)
		return ;
	double latitude = convert_to_decimal((Rational *)&res->latitude, res->north_south);
	double longitude = convert_to_decimal((Rational *)&res->longitude, res->east_west);

	printf("Camera Make: %s\n", res->make);
	printf("Camera Model: %s\n", res->model);
	printf("Latitude: %f\n", latitude);
	printf("Longitude: %f\n", longitude);
}
