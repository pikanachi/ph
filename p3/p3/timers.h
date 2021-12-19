#ifndef TIMERS_H
#define TIMERS_H

#include "gestor_alarmas.h"

/*
 * Inicializa el timer1 para que pueda ser usado
 */
void temporizador_iniciar(void);

/*
 * Inicia la cuenta en el timer1
 */
void temporizador_empezar(void);

/*
 * Resetea el timer 1
 */ 
void reset_timer1(void);

/*
 * Devuelve el tiempo que lleva contado timer1
 */
int temporizador_leer(void);

/*
 * Para el temporizador y devuelve el tiempo transcurrido
 */
int temporizador_parar(void);

/*
 * Programa el timer0 para que encole un evento periódicamente en ms
 */
void temporizador_peridico(int periodo);

#endif
