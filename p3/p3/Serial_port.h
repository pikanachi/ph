#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <stdint.h> 
#include "tableros.h"
#include "sudoku.h"
#include "gestor_energia.h"

enum {
	ESPERA_CMD = 0,
	ESPERA_NRNUM = 1,
	ESPERA_S = 2,
	ESPERA_T = 3,
	ESPERA_EXC = 4,
	ESPERA_E = 5,
	ESPERA_W = 6,
	ESPERA_COL = 7,
	ESPERA_VAL_C = 8,
	ESPERA_CHECK = 9,
};

/*
 * Actualiza el sudoku mostrado en la UART
 */
void UART_actualizar(char *msgFinal);

/*
 * Devuelve 1 si se ha terminado de enviar el string a la UART. 0 en caso contrario
 */
uint8_t UART_ha_terminado(void);

/*
 * Escribe por la UART los candidatos de la celda correspondiente a la fila fil y de la columna col.
 * Si la celda seleccionada era una pista devuelve el mensaje "La celda seleccionada 
 * es una pista"
 */
void UART_enviar_candidatos(int fil, int col);

/* 
 * Procesa la jugada que el usuario quiere introducir en la celda correspondiente a la fila fil y
 * la columna col. Si la celda seleccionada era una pista devuelve el mensaje "La celda en la que 
 * quieres introducir valor es una pista"
 * Si la juagada es valida, envia el mensaje "¿Confirmar jugada?" la realiza, pone una alarma a 3
 * seg para que el usuario valide la juagada y enciende con otra alarma el led de latido.
 */
void UART_introducir_jugada(int fil, int col);

/*
 * Apaga el led de latido de validación con una alarma con su tiempo a 0
 */
void UART_acaba_jugada(void);

/*
 * Si el usuario no valida la jugada, se cancela y muestra el mensaje
 * "Jugada cancelada"
 */
void UART_cancelar_jugada(void);

/*
 * Inicializa las interrupciones de la UART0
 */
void UART_init_serial(void);

/*
 * Manda el string a la UART. Copia el string en un buffer send_buffer y manda el primer caracter
 * del buffer a la UART
 */
void UART_enviar_string(char *string);

/*
 * Borra el tablero y recalcula los candidatos
 */
void UART_borrar_tablero(void);

/*
 * Devuelve 0 si estamos en medio de una jugada, 1 en caso contrario
 */
int en_jugada(void);

/*
 * Prepara la UART para un reinicio de partida
 */
void UART_reset(void);

#endif
