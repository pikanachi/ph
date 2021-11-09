#ifndef IDLE_PWDWN_H
#define IDLE_PWDWN_H

#include <LPC210x.H>
#include "timers.h"

//PARA TESTEAR
#define TIME_PWDN 50000

/*
 * Pone al procesador en modo IDLE para ahorrar energ�a.
 */
void idle(void);

/*
 * Pone al procesador en modo powerDown y s�lo se despertar�
 * por las interrupciones externas marcadas EINT1, EINT2
 */
void pwdwn(void);

#endif
