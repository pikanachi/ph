#ifndef GESTOR_IO_H
#define GESTOR_IO_H

#include "gestor_alarmas.h"
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
 * Vuelve a poner una alarma periodica para el latido tras resetear una partida
 */
void gIO_reset(void);

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
 * Enciende el bit 31 del GPIO
 */
void gIO_encender_latido(void);

/*
 * Apaga el bit 31 del GPIO
 */
void gIO_apagar_latido(void);

/*
 * Alterna el latido del GPIO encendiendo el bit 31 
 * si estaba apagado y apagandolo si estaba encendido
 */
void gIO_alternar_latido(void);

/*
 * Alterna la validacion del GPIO encendiendo el bit 13 
 * si estaba apagado y apagandolo si estaba encendido
 */
void gIO_alternar_validacion(void);

/*
 * Apaga el led de validación (bit 13 GPIO)
 */
void gIO_apagar_validacion(void);

/*
 * Enciende el led de validación (bit 13 GPIO)
 */
void gIO_encender_validacion(void);

#endif
