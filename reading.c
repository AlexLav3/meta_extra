#include "meta_ext.h"

void	read_file(FILE *file)
{
	int buffer[5];
	if (!file)
		return ;
	fread(buffer, sizeof(int), 5, file);
	for (int i = 0; i < 5; i++)
		printf("Element %d: %d\n", i + 1, buffer[i]);
	fclose(file);
}
