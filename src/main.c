
#include <stdlib.h>
#include "sys.h"


int main(void)
{
	void * test = malloc(1);
	while (1) {
		test++;
	}
	free(test);
}
