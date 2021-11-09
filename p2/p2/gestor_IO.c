#include "gestor_IO.h"

/*
 * Inicializa la GPIO
 */
void gIO_inicializar(void) {
	uint16_t candidatos;
  GPIO_iniciar();
	candidatos_actualizar_c(cuadricula_C_C);
	mostrar_candidatos();
}



void mostrar_candidatos(){
	uint16_t candidatos;
	int fila, columna;
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	candidatos = celda_leer_candidatos(cuadricula_C_C[fila][columna]);
	GPIO_escribir(4,9,candidatos);
}
