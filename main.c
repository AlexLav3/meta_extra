#include "meta_ext.h"

int	main(void)
{
    t_data *data = malloc(sizeof(t_data));
    FILE *file = fopen("test.jpg", "rb");
    read_file(file, data);
    return 0; 
}
