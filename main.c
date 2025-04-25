#include "meta_ext.h"

int	main(void)
{
	t_data	*data = malloc(sizeof(t_data));
	t_res	*res = malloc(sizeof(t_res));
	Rational *rational = malloc(sizeof(Rational));
	GPS_Coord *gps_cord = malloc(sizeof(GPS_Coord));

	if(!data || !res || !rational || !gps_cord)
		return 0; 

	FILE	*file = fopen("JPG FILE HERE", "rb");
	read_file(file, data);
    print_res((&data->res_data));

    free(data);
    free(res);
	free(rational);
	free(gps_cord);
	return (0);
}
