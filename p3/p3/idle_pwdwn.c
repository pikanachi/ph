#include "idle_pwdwn.h"



/*
 * Pone al procesador en modo IDLE para ahorrar energía.
 */
void idle(void) {
            				// Se encola el evento   
	PCON |= 0x01;   	// bit 0 = 1, this bit causes the processor clock to be stopped, while on-chip peripherals remain active. 
                    // Any enabled interrupt from a peripheral or an external interrupt source will cause the processor to resume execution.
}

/*
 * Pone al procesador en modo IDLE para ahorrar energía.
 */
void pwdwn(void) {
  EXTWAKE = 6;   		// EXTINT1 will awake the processor and EXTINT2 will awake the processor
	PCON |= 0x02;  	 	// bit 1 = 1, when 1, this bit causes the processor clock to be stopped, while on-chip peripherals remain active. 
                    // Any enabled interrupt from a peripheral or an external interrupt source will cause the processor to resume execution.
}
