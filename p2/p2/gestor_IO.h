#ifndef GESTOR_IO_H
#define GESTOR_IO_H
#include "evento.h"
#include "timers.h"
#include "gpio.h"
#include "tableros.h"
#include "sudoku.h"

/*
 * Inicializa la GPIO
 */
void gIO_inicializar(void);

/*
 *
 */
void gIO_mostrar_candidatos(void);

/*
 *
 */
void gIO_mostrar_valor(void);


void gIO_encender_overflow(void);

int gIO_leer_overflow(void);

void gIO_check_entrada(void);

void gIO_escribir_entrada(void);

void gIO_apagar_validacion(void);

void gIO_encender_validacion(void);
#endif
