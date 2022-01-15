#ifndef BUTTONS_H
#define BUTTONS_H

#include <LPC210x.H>
#include "gestor_pulsacion.h"
#include "evento.h"
#include "timers.h"
#include "idle_pwdwn.h"



/*
 * Habilita las interrupciones EINT1 en el pin P0.14
 */
void eint1_init (void);


/*
 * Habilita las interrupciones EINT2 en el pin P0.15
 */
void eint2_init (void);


/*
 * Habilita interrupciones en VIC
 */
void button_enable_interrupts_1(void);

void button_enable_interrupts_2(void);

/*
 * Devuelven 1 si ha habido una nueva pulsación
 */
unsigned int button_nueva_pulsacion_1(void);

unsigned int button_nueva_pulsacion_2(void);


/*
 * Resetean a 0 las nuevas pulsaciones
 */
void button_clear_nueva_pulsacion_1(void);

void button_clear_nueva_pulsacion_2(void);

#endif
