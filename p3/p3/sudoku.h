#ifndef SUDOKU_H
#define SUDOKU_H

#include "celda.h"
#include "tableros.h"

/* Definiciones para valores muy utilizados */
enum {FALSE = 0, TRUE = 1};

typedef uint16_t CELDA;

/*
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias 
 */
int candidatos_actualizar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/*
 * Devuelve tiempo contado que tarda la funcion candidatos actualizar
 */
int tiempo_actualizar(void);

/*
 * Pone a 0 el tiempo contado que tarda la funcion candidatos actualizar
 */
void reset_tiempo_actualizar(void);

#endif
