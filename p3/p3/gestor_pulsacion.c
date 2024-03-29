#include "gestor_pulsacion.h"

static unsigned int estado_pulsacion_EINT1 = NO_PULSADO;
//static unsigned int estado_pulsacion_EINT2 = NO_PULSADO;

/*
 * Inicializa las interrupciones en EINT1 y EINT2
 */
void gp_inicializar(void) {
    eint1_init();
    //eint2_init();
}

/*
 * Devuelve 1 si el estado es PULSADO. Devuelve 0 en caso contrario
 */
unsigned int gp_leer_pulsacion_1(void){
	unsigned int retVal = 0;
	if(estado_pulsacion_EINT1 == PULSADO){
		retVal = 1;
	}
	return retVal;
}

/*
 * Devuelve el estado de EINT1
 */
unsigned int leer_estado_1(void){
	return estado_pulsacion_EINT1;
}

/*
 * Devuelve el estado de EINT2
 */
/*
unsigned int leer_estado_2(void){
	return estado_pulsacion_EINT2;
}
*/

unsigned int leer_entrada_1(void) {
	EXTINT =  EXTINT | 2; 				// clear interrupt flag de EINT0, EINT1 y EINT2
	if ((EXTINT & 0x2) == 2) {
		return 1;										//El bot�n sigue pulsado
	}
		return 0; 									//El bot�n no est� pulsado
}
/*
unsigned int leer_entrada_2(void) {
	EXTINT =  EXTINT | 1; 				// clear interrupt flag de EINT0, EINT1 y EINT2
	if ((EXTINT & 0x4) == 4) {
		return 1;										//El bot�n sigue pulsado
	}
		return 0; 									//El bot�n no est� pulsado
}
*/

/*
 * Comprueba si se ha levantado la pulsacion y actualiza el estado.
 * si se ha levantado la pulsaci�n vuelve a habilitar las interrupciones
 * en el VIC
 */
void gp_actualizar_estado_EINT1(void) {
	unsigned int estado, entrada;
	int retardo;
	
	
	if (button_nueva_pulsacion_1() == 1) { //ha habido una nueva pulsacion se ajusta el estado
		estado_pulsacion_EINT1 = PULSADO;
	}
	estado = leer_estado_1();
	entrada = leer_entrada_1();
	switch(estado) {
		case PULSADO:
			if (entrada == 0) { // se ha despulsado el boton
				estado_pulsacion_EINT1 = NO_PULSADO;
				button_enable_interrupts_1();
				
				//Cancelar alarma EINT1
				retardo = 0;
				disable_isr_fiq();
				set_Alarma(Check_Pulsacion_EINT1, retardo, 1);
				enable_isr_fiq();
			}
			break;
		case NO_PULSADO:
			if (entrada == 1) { // se ha pulsado
				estado_pulsacion_EINT1 = PULSADO;
			}
			break;
		}
}
/*
void gp_actualizar_estado_EINT2(void) {
	unsigned int estado, entrada;
	int retardo;
	
	
	if (button_nueva_pulsacion_2() == 1) { //ha habido una nueva pulsacion se ajusta el estado
		estado_pulsacion_EINT2 = PULSADO;
	}
	estado = leer_estado_2();
	entrada = leer_entrada_2();
	switch(estado) {
		case PULSADO:
			if (entrada == 0) { // se ha despulsado el boton
				estado_pulsacion_EINT2 = NO_PULSADO;
				button_enable_interrupts_2();
				
				//Cancelar alarma EINT2
				retardo = 0;
				disable_isr_fiq();
				set_Alarma(Check_Pulsacion_EINT2, retardo, 1);
				enable_isr_fiq();
			}
			break;
		case NO_PULSADO:
			if (entrada == 1) { // se ha pulsado
				estado_pulsacion_EINT2 = PULSADO;
			}
			break;
		}
}
*/
