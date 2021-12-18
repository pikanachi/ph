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

void init_serial (void);
int sendchar (int ch) ;
int getchar (void);
void init_serial_byInterrupt(void);
void enviar_string(char *string);
uint8_t ha_terminado(void);
void actualizar_uart(char *msgFinal);
void cancelar_jugada(void);
void acaba_jugada(void);
void uart_enviar_candidatos(int fil, int col);
void uart_introducir_jugada(int fil, int col);
void actualizar_tablero(void);
void uart_borrar_tablero(void);

#endif
