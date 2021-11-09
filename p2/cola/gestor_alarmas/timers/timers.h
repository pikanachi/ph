#ifndef TIMERS_H
#define TIMERS_H

#include "../../LPC210X.H" //<LPC210X.H>
#include "../../cola/cola.h"
#include "../gestor_alarmas.h"

static volatile unsigned int timer1_int_count = 0;

/*
 * Inicializa el timer1 para que pueda ser usado
 */
void temporizador_iniciar(void);

/*
 * Inicia la cuenta en el timer1
 */
void temporizador_empezar(void);

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
