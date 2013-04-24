/*
 * syscalls.c
 *
 *  Created on: 06/10/2010
 *      Author: morten
 */

#include <stddef.h>
#include <sys/types.h>

extern unsigned char * _heap_start;

caddr_t _sbrk ( int incr )
{
  static unsigned char * heap = NULL;
  unsigned char *prev_heap;

  if (heap == NULL) {
    heap = _heap_start;
  }
  prev_heap = heap;
  /* check removed to show basic approach */

  heap += incr;

  return (caddr_t) prev_heap;
}
