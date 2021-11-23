#include <stddef.h>
#include "sudoku.h"

/* *****************************************************************************
 * propaga el valor de una determinada celda en C
 * para actualizar las listas de candidatos
 * de las celdas en su su fila, columna y region
 * Recibe como parametro la cuadricula, y la fila y columna de
 * la celda a propagar; no devuelve nada
 */
 

 


void candidatos_propagar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
	uint8_t fila, uint8_t columna)
{
    uint8_t j, i , init_i, init_j, end_i, end_j;
    // puede ayudar esta "look up table" a mejorar el rendimiento
    const uint8_t init_region[NUM_FILAS] = {0, 0, 0, 3, 3, 3, 6, 6, 6};

    // valor que se propaga
    uint8_t valor = celda_leer_valor(cuadricula[fila][columna]);

    // recorrer fila descartando valor de listas candidatos
    for (j=0;j<NUM_FILAS;j++)
				celda_eliminar_candidato(&cuadricula[fila][j],valor);

    // recorrer columna descartando valor de listas candidatos
    for (i=0;i<NUM_FILAS;i++)
				celda_eliminar_candidato(&cuadricula[i][columna],valor);

    // determinar fronteras región
    init_i = init_region[fila];
    init_j = init_region[columna];
    end_i = init_i + 3; //En verdad el fin es init_i + 3 -1 pero el for es < no <=
    end_j = init_j + 3;

    // recorrer region descartando valor de listas candidatos
    for (i=init_i; i<end_i; i++) {
      for(j=init_j; j<end_j; j++) {
					celda_eliminar_candidato(&cuadricula[i][j],valor);
	    }
    }
}


/* *****************************************************************************
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias */

/* Init del sudoku en codigo C invocando a propagar en C
 * Recibe la cuadricula como primer parametro
 * y devuelve en celdas_vacias el n?mero de celdas vacias
 */
int candidatos_actualizar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS])
{
  int celdas_vacias = 0;
  uint8_t i;
  uint8_t j;

	//borrar todos los candidatos
	for (i=0; i < NUM_FILAS; i++) {
		for (j=0; j < NUM_FILAS; j++) {
			if (esVacia(cuadricula[i][j])) {
				elminaCandidatos(&cuadricula[i][j]);
			}
		}
	}

	//recalcular candidatos de las celdas vacias calculando cuantas hay vacias
	for (i=0; i < NUM_FILAS; i++) {
		for (j=0; j < NUM_FILAS; j++) {
			if (esVacia(cuadricula[i][j])) {
				celdas_vacias++;
			} else {
				//ORIGINAL candidatos_propagar_c(cuadricula, i, j);
					candidatos_propagar_c(cuadricula, i, j);
			}
		}
	}

	//retornar el numero de celdas vacias
	return celdas_vacias;
}
