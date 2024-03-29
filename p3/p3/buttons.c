#include "buttons.h"

#define TIME_AL_PUL 100

static volatile int nueva_pulsacion_eint1;
//static volatile int nueva_pulsacion_eint2;

/*
 * ISR para las EINT1
 */
void eint1_ISR (void) __irq {
	Evento ePulsacion;
	int retardo;
	disable_isr_fiq();
	EXTINT =  EXTINT | 0x2;      		  								// clear interrupt flag
	VICVectAddr = 0;                     							// Acknowledge Interrupt
	VICIntEnClr = 0x00008000;    											// deshabilitar eint1 pone a 1 bit 15	
	nueva_pulsacion_eint1 = 1;
	
	//Encolar alarma EINT1 a 100 ms
  retardo = TIME_AL_PUL & 0x007FFFFF;     					// Asegurarnos que el retardo es de 23bits
	set_Alarma(Check_Pulsacion_EINT1, retardo, 1);    // Se encola el evento   
	
	//Actualizar contador para PWDOWN
	retardo = TIME_PWDN & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
	set_Alarma(Power_Down, retardo, 1);
	//Encolar evento de pulsacion
  ePulsacion.ID_evento = Pulsacion_EINT1;
  //ePulsacion.timestamp = temporizador_leer() / 1000;
  cola_guardar_evento(ePulsacion); 
	enable_isr_fiq();
}

/*
 * Devuelven 1 si ha habido una nueva pulsación
 */
unsigned int button_nueva_pulsacion_1(void){
	return nueva_pulsacion_eint1;
}

/*
 * Resetean a 0 las nuevas pulsaciones
 */
void button_clear_nueva_pulsacion_1(void){
	nueva_pulsacion_eint1 = 0;
}

/*
 * Habilita interrupciones en VIC
 */
void button_enable_interrupts_1(void) {
  VICIntEnable = VICIntEnable | 0x00008000;
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

