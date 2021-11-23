#include "buttons.h"

#define TIME_AL_PUL 100



static volatile int nueva_pulsacion_eint1;
static volatile int nueva_pulsacion_eint2;

/*
 * ISR para las EINT1
 */
void eint1_ISR (void) __irq {
	Evento eAlarma;
	Evento ePulsacion;
	int retardo;
	VICIntEnClr = 0x00008000;    											// deshabilitar eint1 pone a 1 bit 15	
	EXTINT =  EXTINT | 0x2;      		  								// clear interrupt flag
	VICVectAddr = 0;                     							// Acknowledge Interrupt

	nueva_pulsacion_eint1 = 1;
	
	//Encolar alarma EINT1 a 100 ms
  retardo = TIME_AL_PUL & 0x007FFFFF;     					// Asegurarnos que el retardo es de 23bits
  eAlarma.ID_evento = Set_Alarma;
  eAlarma.auxData = Check_Pulsacion_EINT1;  				// ID evento a generar
  eAlarma.auxData = eAlarma.auxData << 1;
  eAlarma.auxData = eAlarma.auxData | 1;          	// Es periódica
  eAlarma.auxData = eAlarma.auxData << 23;
  eAlarma.auxData = eAlarma.auxData | retardo;
  eAlarma.timestamp = temporizador_leer() / 1000;
  cola_guardar_evento(eAlarma);             				// Se encola el evento   
	
	//Actualizar contador para PWDOWN
	retardo = TIME_PWDN & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
	eAlarma.ID_evento = Set_Alarma;
	eAlarma.auxData = Power_Down;  										// ID evento a generar
	eAlarma.auxData = eAlarma.auxData << 1;
	eAlarma.auxData = eAlarma.auxData | 1;          	// Es periódica
	eAlarma.auxData = eAlarma.auxData << 23;
	eAlarma.auxData = eAlarma.auxData | retardo;
	eAlarma.timestamp = temporizador_leer() / 1000;
	cola_guardar_evento(eAlarma); 
	//Encolar evento de pulsacion
  ePulsacion.ID_evento = Pulsacion_EINT1;
  ePulsacion.timestamp = temporizador_leer() / 1000;
  cola_guardar_evento(ePulsacion); 
}


/*
 * ISR para las EINT2
 */
void eint2_ISR (void) __irq {
	Evento eAlarma;
	Evento ePulsacion;
	int retardo;
	
	VICIntEnClr = 0x00010000;    											// deshabilitar eint2 pone a 1 bit 16
	EXTINT =  EXTINT | 0x4;        										// clear interrupt flag   
	nueva_pulsacion_eint2 = 1;
	
	//Encolar alarma EINT1 a 100 ms
  retardo = TIME_AL_PUL & 0x007FFFFF;     					// Asegurarnos que el retardo es de 23bits
  eAlarma.ID_evento = Set_Alarma;
  eAlarma.auxData = Check_Pulsacion_EINT2;  				// ID evento a generar
  eAlarma.auxData = eAlarma.auxData << 1;
  eAlarma.auxData = eAlarma.auxData | 1;         		// Es periódica
  eAlarma.auxData = eAlarma.auxData << 23;
  eAlarma.auxData = eAlarma.auxData | retardo;
  eAlarma.timestamp = temporizador_leer() / 1000;
	cola_guardar_evento(eAlarma); 
	
	//Encolar evento de pulsacion
  ePulsacion.ID_evento = Pulsacion_EINT2;
  ePulsacion.timestamp = temporizador_leer() / 1000;
  cola_guardar_evento(ePulsacion); 
	
	//Actualizar contador para PWDOWN
	retardo = TIME_PWDN & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
	eAlarma.ID_evento = Set_Alarma;
	eAlarma.auxData = Power_Down;  										// ID evento a generar
	eAlarma.auxData = eAlarma.auxData << 1;
	eAlarma.auxData = eAlarma.auxData | 1;          	// Es periódica
	eAlarma.auxData = eAlarma.auxData << 23;
	eAlarma.auxData = eAlarma.auxData | retardo;
	eAlarma.timestamp = temporizador_leer() / 1000;
	cola_guardar_evento(eAlarma); 
	VICVectAddr = 0;                     							// Acknowledge Interrupt
}

/*
 * Devuelven 1 si ha habido una nueva pulsación
 */
unsigned int button_nueva_pulsacion_1(void){
	return nueva_pulsacion_eint1;
}

unsigned int button_nueva_pulsacion_2(void){
	return nueva_pulsacion_eint2;
}

/*
 * Resetean a 0 las nuevas pulsaciones
 */
void button_clear_nueva_pulsacion_1(void){
	nueva_pulsacion_eint1 = 0;
}

void button_clear_nueva_pulsacion_2(void){
	nueva_pulsacion_eint2 = 0;
}


/*
 * Habilita interrupciones en VIC
 */
void button_enable_interrupts_1(void) {
  VICIntEnable = VICIntEnable | 0x00008000;
}

void button_enable_interrupts_2(void) {
  VICIntEnable = VICIntEnable | 0x00010000;
}



/*
 * Habilita las interrupciones EINT1 en el pin P0.14
 */
void eint1_init (void) {
	nueva_pulsacion_eint1 = 0;
	EXTINT =  EXTINT | 0x2;     	                  // clear interrupt flag	
	
  // configuration of the IRQ slot number 1 of the VIC for EXTINT1
	VICVectAddr1 = (unsigned long)eint1_ISR;
  button_enable_interrupts_1();                   // Enable EXTINT1 Interrupt
  //Pinsel ver man (pg 64)
	PINSEL0 = PINSEL0 & 0xCFFFFFFF;	                // Sets bits 28 and 29 to 00.
  PINSEL0 = PINSEL0 | 0x20000000;                 // To enable EINT1 interrupt 10, pin P0.14
	VICVectCntl1 = 0x20 | 15;                       // 0x20 bit 5 enables vectored IRQs. Number 15 is the EINT1
}


/*
 * Habilita las interrupciones EINT2 en el pin P0.15
 */
void eint2_init (void) {
	nueva_pulsacion_eint2 = 0;
	EXTINT =  EXTINT | 0x4; 	                      // clear interrupt flag	
	
  // configuration of the IRQ slot number 3 of the VIC for EXTINT2
	VICVectAddr2 = (unsigned long)eint2_ISR;
    
  //Pinsel ver man (pg 64)
	PINSEL0 = PINSEL0 & 0x3FFFFFFF;	                // Sets bits 30 and 31 to 00.
  PINSEL0 = PINSEL0 | 0x80000000;                 // To enable EINT2 interrupt 10, pin P0.14
	VICVectCntl2 = 0x20 | 16;                       // 0x20 bit 5 enables vectored IRQs. Number 16 is the EINT2
  button_enable_interrupts_2();                   // Enable EXTINT2 Interrupt
}
