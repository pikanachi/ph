#ifndef SOFTI_H
#define SOFTI_H

#include "inttypes.h"
#include "timers.h"

void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);
void __swi(0xFD) enable_isr_fiq (void);
void __swi(0xFC) disable_isr_fiq (void);

uint32_t __swi(0) clock_gettime(void);


#endif
