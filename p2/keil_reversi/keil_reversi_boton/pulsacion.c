#include "pulsacion.h"
#include <LPC210x.H>                       /* LPC210x definitions */
#include "boton_eint0.h"

static unsigned int estado_pulsacion = NO_PULSADO;


void clear_nueva_pulsacion(void){
	eint0_clear_nueva_pulsacion();
}

unsigned int nueva_pulsacion(void){
	unsigned int new;
	new = eint0_read_nueva_pulsacion();
	eint0_clear_nueva_pulsacion(); // Las pulsaciones sólo deben procesarse una vez. Por tanto se pone a 0 después de leerlo
	return new;
}

unsigned int leer_entrada(void) {
	EXTINT =  EXTINT | 1; // clear interrupt flag de EINT0  
	if ((EXTINT & 0x1) == 1) {
		return 1;	//El botón sigue pulsado
	}
		return 0; //El botón no esá pulsado
}

unsigned int leer_estado(void){
	return estado_pulsacion; 
}

void actualizar_estado(void) {
	unsigned int estado, entrada;
	
	if (nueva_pulsacion() == 1) { //ha habido una nueva pulsacion se ajusta el estado
		estado_pulsacion = PULSADO;
	}
	estado = leer_estado();
	entrada = leer_entrada();
	switch(estado) {
		case PULSADO:
			if (entrada == 0) { // se ha despulsado el boton
				estado_pulsacion = NO_PULSADO;
				VICIntEnable = VICIntEnable | 0x00004000; //Enable EXTINT0 Interruption
			}
			break;
		case NO_PULSADO:
			if (entrada == 1) { // se ha pulsado
				estado_pulsacion = PULSADO;
			}
			break;
		}

}
