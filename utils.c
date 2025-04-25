#include "meta_ext.h"

double rational_to_double(Rational number)
{
    return number.denominator == 0 ? 0.0 : (double)number.numerator / number.denominator;
}

double convert_to_decimal(Rational *coords, char ref)
{
    double deg = rational_to_double(coords[0]);
    double min = rational_to_double(coords[1]);
    double sec = rational_to_double(coords[2]);

    double result = deg + min / 60.0 + sec / 3600.0;
    if (ref == 'S' || ref == 'W') result *= -1.0;
    return result;
}

GPS_Coord 			convert_coord(Rational *coords)
{
    GPS_Coord result;
    result.degrees = coords[0];
    result.minutes = coords[1];
    result.seconds = coords[2];
    return result;
}