#include "meta_ext.h"

int	main(void)
{
    t_data *data = malloc(sizeof(t_data));
    t_res *res = malloc(sizeof(t_res));
    FILE *file = fopen("20250423_182131.jpg", "rb");
   if(read_file(file, data))
        make_str(file, data, res);
    print_res(res);
    free(data);
    free(res);
    return 0; 
}
