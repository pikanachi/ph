#ifndef GESTOR_IO_H
#define GESTOR_IO_H

#include "evento.h"
#include "timers.h"
#include "gpio.h"
#include "tableros.h"
#include "sudoku.h"

/*
 * Inicializa el gestor_IO iniciclizandi la GPIO y pone una alarma periodica
 * que Check_Entrada para mostrar en el GPIO el valor de la celda seleccionada
 * (valor y candidatos)
 */
void gIO_inicializar(void);

/*
 * Muestra el valor de los candidatos de la celda seleccionada (fila y columna) 
 * en el GPIO
 */
void gIO_mostrar_candidatos(void);

/*
 * Muestra el valor de la celda seleccionada (fila y columna) en el GPIO
 */
void gIO_mostrar_valor(void);

/*
 * Escribe el bit de overfow del GPIO para saber si ha desbordado la
 * cola de eventos
 */
void gIO_encender_overflow(void);

/*
 * Lee el estado del bit de overfow del GPIO para saber si ha desbordado la
 * cola de eventos
 */
int gIO_leer_overflow(void);

/*
 * Si la fila y la columna introducidas por el usuario es invalida (>9)
 * se enciende el led de validación (entrada invaida) sino si no tiene valor
 * la celda se enciende el led de validacion. Finalmente se muestra el
 * valor de la casilla seleccionada y sus candidatos en el GPIO
 */
void gIO_check_entrada(void);

/*
 * Si la fila y la columna introducidas por el usuario es invalida (>9)
 * se enciende el led de validación (entrada invaida) sino, si el valir introcucido
 * por el usuario es válido (>= 1 y <= 9) y es uno de los posibles candidatos de la 
 * celda,se enciende el led de validación durante 1000ms, se pone el valor introducido
 * en la celda seleccionada, se actualizan los candidatos y se muestran en el GPIO los
 * candidatos y el valor introducidos
 */
void gIO_escribir_entrada(void);

/*
 * Apaga el led de validación (bit 13 GPIO)
 */
void gIO_apagar_validacion(void);

/*
 * Enciende el led de validación (bit 13 GPIO)
 */
void gIO_encender_validacion(void);

#endif
