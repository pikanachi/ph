#include "gestor_energia.h"

/*
 * Inicia la alarma para poner al procesador a PWDOWN
 */
void ge_inicializar(void){
		int retardo;
		retardo = TIME_PWDN & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
		disable_isr_fiq();
		set_Alarma(Power_Down, retardo, 1);
		enable_isr_fiq();
}
/*
 * Pone al procesador en modo IDE
 */
void ge_modo_IDE(void) {
    //Se va a modo idle
    idle();
}

/*
 * Pone al procesador en modo powerdown
 */
void ge_modo_pwdwn(void) {
		
    pwdwn();
}
