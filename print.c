#include "meta_ext.h"

void print_res(t_res *res) {
  if (!res)
    return;

  double longitude = 0;
  double latitude = 0;

  if(res->north_south)
    latitude = convert_to_decimal((Rational *)&res->latitude, res->north_south);
  if(res->east_west)
    longitude = convert_to_decimal((Rational *)&res->longitude, res->east_west);
  printf("Camera Make: %s\n", res->make);
  printf("Camera Model: %s\n", res->model);
  printf("Latitude: %f\n", latitude);
  printf("Longitude: %f\n", longitude);
}
