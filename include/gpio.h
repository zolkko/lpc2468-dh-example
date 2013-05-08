
#ifndef _gpio_h_
#define _gpio_h_

#define EINT0_bp 0
#define EINT0_bm (1 << EINT0_bp)

#define EINT1_bp 1
#define EINT1_bm (1 << EINT1_bp)

#define EINT2_bp 2
#define EINT2_bm (1 << EINT2_bp)

#define EINT3_bp 3
#define EINT3_bm (1 << EINT3_bp)

#define EXTMODE0_bp 0
#define EXTMODE0_bm (1 << EXTMODE0_bp)

#define EXTMODE1_bp 1
#define EXTMODE1_bm (1 << EXTMODE1_bp)

#define EXTMODE2_bp 2
#define EXTMODE2_bm (1 << EXTMODE2_bp)

#define EXTMODE3_bp 3
#define EXTMODE3_bm (1 << EXTMODE3_bp)

#define EXTMODE_SENS_LEVEL_gc 0
#define EXTMODE_SENS_EDGE_gc 1

#define EXTPOLAR0_bp 0
#define EXTPOLAR0_bm (1 << EXTPOLAR0_bp)

#define EXTPOLAR1_bp 1
#define EXTPOLAR1_bm (1 << EXTPOLAR1_bp)

#define EXTPOLAR2_bp 2
#define EXTPOLAR2_bm (1 << EXTPOLAR2_bp)

#define EXTPOLAR3_bp 3
#define EXTPOLAR3_bm (1 << EXTPOLAR3_bp)


void gpio_init(void * handler);


#endif
