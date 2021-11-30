#ifndef GESTOR_PULSACION_H
#define GESTOR_PULSACION_H

#include "buttons.h"
#include "gestor_alarmas.h"

enum {
	PULSADO = 0,
	NO_PULSADO = 1
};

unsigned int gp_leer_pulsacion_1(void);

/*
 * Inicializa las interrupciones en EINT1 y EINT2
 */
void gp_inicializar(void);

/*
 * Comprueba si se ha levantado la pulsacion y actualiza el estado.
 * si se ha levantado la pulsación vuelve a habilitar las interrupciones
 * en el VIC
 */
void gp_actualizar_estado_EINT1(void);

void gp_actualizar_estado_EINT2(void);

#endif
