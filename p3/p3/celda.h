/* guarda para evitar inclusiones m�ltiples (include guard) */
#ifndef CELDA_H
#define CELDA_H

#include <inttypes.h>

 enum {NUM_FILAS = 9,
      PADDING = 7,
      NUM_COLUMNAS = NUM_FILAS + PADDING};

/* Cada celda se codifica en 16 bits
 * bits [15,7]: los 9 bits m�s significativos representan el vector de candidatos,
 * si el bit 7 + valor - 1 est� a 0, valor es candidato, 1 en caso contrario
 * bit 6: no empleado
 * bit 5: error
 * bit 4: pista
 * bits [3,0]: valor de la celda
 */

enum { BIT_CANDIDATOS = 7 };

typedef uint16_t CELDA;

/* *****************************************************************************
 * elimina el candidato del valor almacenado en la celda indicada */
__inline static void celda_eliminar_candidato(CELDA *celdaptr, uint8_t valor)
{
    *celdaptr = *celdaptr | (0x0001 << (BIT_CANDIDATOS + valor - 1));
}

/* *****************************************************************************
 * modifica el valor almacenado en la celda indicada */
__inline static void
celda_poner_valor(CELDA *celdaptr, uint8_t val)
{
    *celdaptr = (*celdaptr & 0xFFF0) | (val & 0x000F);
}

/* *****************************************************************************
 * extrae el valor almacenado en los 16 bits de una celda */
__inline static uint8_t
celda_leer_valor(CELDA celda)
{
    return (celda & 0x000F);
}

__inline static uint16_t
celda_leer_candidatos(CELDA celda)
{
    return (celda & 0xFF80) >> 7;
}

/* *****************************************************************************
 * devuelve 0 si y s�lo s� celda_leer_valor(celda) = 0 */
__inline static uint8_t esVacia(CELDA celda)
{
	return celda_leer_valor(celda) == 0;
}

/* *****************************************************************************
 * modifica la celda poniendo a 0 todos sus candidatos (todos son candidatos) */
static void elminaCandidatos(CELDA *celdaptr) {
	*celdaptr = *celdaptr & 0x007F;
}

static int esPista(CELDA celda){
	return celda & 0x0010;
}

static int esError(CELDA celda){
	return celda & 0x0020;
}

static int celda_noEsCandidato(CELDA celda, uint8_t valor){
	int mask = 1 << (6 + valor);
	mask = mask & celda;
	return mask >> (6 + valor);
}
#endif // CELDA_H
