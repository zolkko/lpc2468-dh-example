/*
 * taken from newlib FAQ
 * TODO: test for stack interseption.
 */

#include <stddef.h>
#include <sys/types.h>
#include <errno.h>

extern int _heap_start;
extern int errno;

caddr_t _sbrk (int incr )
{
  static unsigned char * heap = NULL;
  unsigned char *prev_heap;

  if (heap == NULL) {
    heap = (unsigned char *)&_heap_start;
  }

  if ((heap + incr) > (&_heap_start + 1024)) {
	  errno = ENOMEM;
	  return (caddr_t) - 1;
  }

  prev_heap = heap;

  heap += incr;

  return (caddr_t) prev_heap;
}
