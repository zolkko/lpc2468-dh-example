#include "config.h"
#include <stdlib.h>
#include <stdint.h>
#include "cirbuf.h"

cirbuf_t * cirbuf_new(cirbuf_size_t size)
{
	cirbuf_t * buf = (cirbuf_t *)malloc(sizeof(cirbuf_t));
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

void cirbuf_init(cirbuf_t * buf, uint8_t * buffer, cirbuf_size_t size)
{
	buf->buff = buffer;
	buf->size = size;
	buf->ridx = 0;
	buf->widx = 0;
}

uint8_t cirbuf_write(cirbuf_t * buf, uint8_t value)
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

uint8_t cirbuf_read(cirbuf_t * buf, uint8_t * value)
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

uint8_t cirbuf_is_empty(const cirbuf_t * buf)
{
	if (buf->widx == buf->ridx) {
		return 1;
	} else {
		return 0;
	}
}

cirbuf_size_t cirbuf_length(const cirbuf_t * buf)
{
	if (buf->widx == buf->ridx) {
		return 0;
	} else if (buf->widx > buf->ridx) {
		return buf->widx - buf->ridx;
	} else {
		return (buf->ridx - buf->widx) + (buf->size - buf->ridx);
	}
}

uint8_t cirbuf_can_write(const cirbuf_t * buf)
{
	return cirbuf_length(buf) < buf->size;
}
