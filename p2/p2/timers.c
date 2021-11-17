#include "timers.h"

#define frecuencia 60                                   //Frecuencia del procesador
#define ticks_by_msec 61476

static volatile unsigned int timer1_int_count = 0;

/*
 * ISR para el timer0 (temp periódico)
 */
void timer0_ISR (void) __irq {
    //Cuando llega el temporizador periódico, encola el evento de temporizador periódico
    ga_encolar_evento_temp();
		T0IR = 1;
		VICVectAddr = 0;
}

/*
 * ISR para el timer1
 */
void timer1_ISR (void) __irq {
    timer1_int_count++;
		T1IR = 1;
		VICVectAddr = 0;
}

/*
 * Inicializa el timer1 para que pueda ser usado
 */
void temporizador_iniciar(void) {
    VPBDIV = 0x00000001;                               //Para que no divida la frecuencia del reloj

		T1MR0 = 0xFFFFFFFF;                                //Se pone el mach register a 2^32-1 (max valor ticks)
    T1MCR = 3;                                         // Generates an interrupt and resets the count when the value of MR0 is reached
		T1TCR = 1;
		VICVectAddr3 = (unsigned long)timer1_ISR;          // set the RSI
   
    // 0x20 bit 5 enables vectored IRQs. 
    // 5 is the number of the interrupt assigned. Number 5 is the Timer 1 (see table 40 of the LPC2105 user manual  
		VICVectCntl3 = 0x20 | 5;   
    VICIntEnable = VICIntEnable | 0x00000020;          // Enable Timer1 Interrupt
}

/*
 * Inicia la cuenta en el timer1
 */
void temporizador_empezar(void) {
    T1TCR = 1;                                         // Timer1 Enable
}

/*
 * Devuelve el tiempo que lleva contado timer1
 */
int temporizador_leer(void) {
    int tiempo = (timer1_int_count * (0xFFFFFFFF / ticks_by_msec)) * 1000;  //tiempo en us
    int pico = (T1TC/ticks_by_msec) * (1000);                      //pico que se suma(lo que llevamos)

    return tiempo + pico;
}

/*
 * Para el temporizador y devuelve el tiempo transcurrido
 */
int temporizador_parar(void) {
    T1TCR = 0;                                          // Timer1 Disable
    //VICIntEnable = VICIntEnable & 0xFFFFFFDF;         // Enable Timer1 Interrupt
    return temporizador_leer();
}

/*
 * Programa el timer0 para que encole un evento periódicamente en ms
 */
void temporizador_peridico(int periodo) {
	//periodo en ms se pasa a ticks
    T0MR0 = (periodo * ticks_by_msec);          //Se pone el mach register a los ticks que ha de llegar según el período
    T0MCR = 3;                                         // Generates an interrupt and resets the count when the value of MR0 is reached
		T0TCR = 1;
		VICVectAddr0 = (unsigned long)timer0_ISR;          // set the RSI
   
    // 0x20 bit 5 enables vectored IRQs. 
    // 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl0 = 0x20 | 4;   
    VICIntEnable = VICIntEnable | 0x00000010;          // Enable Timer0 Interrupt
}
