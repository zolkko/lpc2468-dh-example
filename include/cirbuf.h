
#ifndef _cirbuf_h_
#define _curbuf_h_

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t cirbuf_size_t;

typedef struct cirbuf {
	uint8_t * buff;     // buffer pointer
	cirbuf_size_t size; // buffer size
	uint32_t ridx;      // read index
	uint32_t widx;      // write index
} cirbuf_t;

#ifdef HAS_MALLOC
/**
 * Allocates buffer.
 */
cirbuf_t * cirbuf_new(cirbuf_size_t size);

/**
 * Frees allocated buffer.
 */
void cirbuf_free(cirbuf_t * buf);

#endif /* HAS_MALLOC */

void cirbuf_init(cirbuf_t * buf, uint8_t * buffer, cirbuf_size_t size);

/**
 * Writes data into circular buffer.
 */
uint8_t cyrbuf_write(cirbuf_t * buf, uint8_t value);

/**
 * Reads data from circular buffer
 */
uint8_t cyrbuf_read(cirbuf_t * buf, uint8_t * value);

/**
 * Tests circular buffer whether it empty or not.
 */
uint8_t cyrbuf_is_empty(const cirbuf_t * buf);

#ifdef __cplusplus
}
#endif

#endif