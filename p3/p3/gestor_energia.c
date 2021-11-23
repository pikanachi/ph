#include "gestor_energia.h"


/*
 * Inicia la alarma para poner al procesador a PWDOWN
 */
void ge_inicializar(void){
		int retardo;
		Evento eAlarma;
		retardo = TIME_PWDN & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
		eAlarma.ID_evento = Set_Alarma;
		eAlarma.auxData = Power_Down;  							// ID evento a generar
		eAlarma.auxData = eAlarma.auxData << 1;
		eAlarma.auxData = eAlarma.auxData | 1;          	// Es periódica
		eAlarma.auxData = eAlarma.auxData << 23;
		eAlarma.auxData = eAlarma.auxData | retardo;
		eAlarma.timestamp = temporizador_leer() / 1000;
		cola_guardar_evento(eAlarma); 
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
