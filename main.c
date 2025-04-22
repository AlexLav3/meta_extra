#include "meta_ext.h"

int	main(void)
{
    FILE *file = fopen("images.jpeg", "rb");
    read_file(file);
    return 0; 
}
