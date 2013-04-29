
#include <stdint.h>
#include <stdlib.h>
#include "sys.h"
#include <lpc24xx.h>


int main(void)
{
	uint8_t * a = NULL;
	uint8_t * b = NULL;

	a = (uint8_t *) malloc(sizeof(uint8_t));
	*a = 0x01;

	b = (uint8_t *) malloc(sizeof(uint8_t));
	*b = 0x02;

	free(a);
	free(b);

	int test = 0;
	while (1) {
		test++;
	}
}
