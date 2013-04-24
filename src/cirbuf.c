#include "config.h"
#ifdef HAS_MALLOC
#include <stdlib.h>
#endif
#include <stdint.h>
#include "cirbuf.h"

#ifdef HAS_MALLOC
cirbuf_t * cyrbuf_new(cirbuf_size_t size)
{
	cirbuf_t * buf = (cirbuf_t *)malloc(sizeof(cirbuf_size_t));
	buf->size = size;
	buf->ridx = 0;
	buf->widx = 0;
	buf->buff = (uint8_t *)malloc(size * sizeof(uint8_t));
	return buf;
}

void cirbuf_free(cirbuf_t * buf)
{
	if (buf != NULL) {
		if (buf->buff != NULL) {
			free(buf->buff);
			buf->buff = NULL;
		}
		free(buf);
	}
}
#endif

void cirbuf_init(cirbuf_t * buf, uint8_t * buffer, cirbuf_size_t size)
{
	buf->buff = buffer;
	buf->size = size;
	buf->ridx = 0;
	buf->widx = 0;
}

uint8_t cyrbuf_write(cirbuf_t * buf, uint8_t value)
{
	buf->buff[buf->widx] = value;
	if (buf->widx == (buf->size - 1)) {
		buf->widx = 0;
	} else {
		buf->widx++;
	}

	if (buf->widx == buf->ridx) {
		if (buf->ridx == (buf->size - 1)) {
			buf->ridx = 0;
		} else {
			buf->ridx++;
		}
		return 0;
	} else {
		return 1;
	}
}

uint8_t cyrbuf_read(cirbuf_t * buf, uint8_t * value)
{
	if (buf->ridx == buf->widx) {
		return 0;
	} else {
		*value = buf->buff[buf->ridx];
		if (buf->ridx == (buf->size - 1)) {
			buf->ridx = 0;
		} else {
			buf->ridx++;
		}
		return 1;
	}
}

uint8_t cyrbuf_is_empty(const cirbuf_t * buf)
{
	if (buf->widx == buf->ridx) {
		return 1;
	} else {
		return 0;
	}
}
