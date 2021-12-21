#ifndef BUTTONS_H
#define BUTTONS_H

#include <LPC210x.H>
#include "gestor_pulsacion.h"
#include "gestor_alarmas.h"
#include "idle_pwdwn.h"

/*
 * Habilita las interrupciones EINT1 en el pin P0.14
 */
void eint1_init (void);

/*
 * Habilita interrupciones en VIC
 */
void button_enable_interrupts_1(void);

/*
 * Devuelven 1 si ha habido una nueva pulsación
 */
unsigned int button_nueva_pulsacion_1(void);

/*
 * Resetean a 0 las nuevas pulsaciones
 */
void button_clear_nueva_pulsacion_1(void);

#endif
