#ifndef SUDOKU_H
#define SUDOKU_H

#include "celda.h"

/* Definiciones para valores muy utilizados */
enum {FALSE = 0, TRUE = 1};

typedef uint16_t CELDA;

int candidatos_actualizar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

int tiempo_actualizar(void);

void reset_tiempo_actualizar(void);

#endif
