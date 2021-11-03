#include "idle_pwdwn.h"

#define TIME_PWDN 15000

/*
 * Pone al procesador en modo IDLE para ahorrar energía.
 */
void idle(void) {
	//Crear evento set alarma para que en 15segs me meta en pwdwn
	//Si llega cualquier otro evento se resetea la cuenta
	int retardo;
	Evento eAlarma;
	
	//Encolar alarma EINT1 a 100 ms
  retardo = TIME_PWDN & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
  eAlarma.ID_evento = Set_Alarma;
  eAlarma.auxData = Power_Down;  							// ID evento a generar
  eAlarma.auxData = eAlarma.auxData << 1;
  eAlarma.auxData = eAlarma.auxData | 1;          	// Es periódica
  eAlarma.auxData = eAlarma.auxData << 23;
  eAlarma.auxData = eAlarma.auxData | retardo;
  eAlarma.timestamp = temporizador_leer() / 1000;
  cola_guardar_evento(eAlarma);             				// Se encola el evento   
	
	PCON |= 0x01;   	// bit 0 = 1, this bit causes the processor clock to be stopped, while on-chip peripherals remain active. 
                    // Any enabled interrupt from a peripheral or an external interrupt source will cause the processor to resume execution.
}

/*
 * Pone al procesador en modo IDLE para ahorrar energía.
 */
void pwdwn(void) {
  EXTWAKE1 = 1;   // EXTINT1 will awake the processor
  EXTWAKE2 = 1;   // EXTINT2 will awake the processor
	PCON |= 0x02;   // bit 1 = 1, when 1, this bit causes the processor clock to be stopped, while on-chip peripherals remain active. 
                    // Any enabled interrupt from a peripheral or an external interrupt source will cause the processor to resume execution.
}
