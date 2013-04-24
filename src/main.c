
#include <stdlib.h>
#include "sys.h"


int main(void)
{
	int * test = malloc(1);
	*test = 0;

	int * test2 = malloc(1);
	*test2 = 1;
	*test = 0xbad0;
	free(test2);
	free(test);
	test = NULL;
	test2 = NULL;

	// int test = 0;
	while (1) {
		test++;
	}
	// free(test);
}
