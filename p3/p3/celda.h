/* guarda para evitar inclusiones múltiples (include guard) */
#ifndef CELDA_H
#define CELDA_H

#include <inttypes.h>

 enum {NUM_FILAS = 9,
      PADDING = 7,
      NUM_COLUMNAS = NUM_FILAS + PADDING};

/* Cada celda se codifica en 16 bits
 * bits [15,7]: los 9 bits más significativos representan el vector de candidatos,
 * si el bit 7 + valor - 1 está a 0, valor es candidato, 1 en caso contrario
 * bit 6: no empleado
 * bit 5: error
 * bit 4: pista
 * bits [3,0]: valor de la celda
 */

enum { BIT_CANDIDATOS = 7 };

typedef uint16_t CELDA;

/*
 * Elimina el candidato del valor almacenado en la celda indicada 
 */
__inline static void celda_eliminar_candidato(CELDA *celdaptr, uint8_t valor) {
    *celdaptr = *celdaptr | (0x0001 << (BIT_CANDIDATOS + valor - 1));
}

/*
 * Modifica el valor almacenado en la celda indicada 
 */
__inline static void celda_poner_valor(CELDA *celdaptr, uint8_t val) {
    *celdaptr = (*celdaptr & 0xFFF0) | (val & 0x000F);
}

/*
 * Extrae el valor almacenado en los 16 bits de una celda 
 */
__inline static uint8_t celda_leer_valor(CELDA celda) {
    return (celda & 0x000F);
}

/*
 * Devuelve los candidatos de una celda 
 */
__inline static uint16_t celda_leer_candidatos(CELDA celda) {
    return (celda & 0xFF80) >> 7;
}

/*
 * Devuelve 0 si y sólo sí celda_leer_valor(celda) = 0 
 */
__inline static uint8_t esVacia(CELDA celda) {
	return celda_leer_valor(celda) == 0;
}

/*
 * Modifica la celda poniendo a 0 todos sus candidatos (todos son candidatos) 
 */
__inline static void elminaCandidatos(CELDA *celdaptr) {
	*celdaptr = *celdaptr & 0x007F;
}

/*
 * Devuelve 1 si la celda seleccionada es pista. 0 en caso contrario
 */
__inline static int esPista(CELDA celda) {
	return celda & 0x0010;
}

/*
 * Devuelve 1 si la celda seleccionada es error. 0 en caso contrario
 */
__inline static int esError(CELDA celda) {
	return celda & 0x0020;
}

/*
 * Pone a 0 el bit de error de la celda *celdaptr
 */
__inline void celda_borrarError(CELDA *celdaptr) {
	*celdaptr = *celdaptr & 0xFFFFFFDF;
}

/*
 * Pone a 1 el bit de error de la celda *celdaptr
 */
__inline void celda_ponerError(CELDA *celdaptr) {
	*celdaptr = *celdaptr | 0x20;
}

/*
 * Devuelve 1 si no es candidato valor en la celda. 0 en caso contrario
 */
static int celda_noEsCandidato(CELDA celda, uint8_t valor){
	int mask = 1 << (6 + valor);
	mask = mask & celda;
	return mask >> (6 + valor);
}

static void checkError(CELDA *celda, uint8_t old_valor){
	int mask = 0xF;
	int valor = mask & *celda;
	if(valor == 0){
		mask = 1 << 5;
		mask = ~mask;
		*celda = *celda & mask;
	} else{
		if(old_valor != valor){
			mask = 1 <<(6 + valor);
			mask = mask & *celda;
			mask = mask >> (6 + valor);
			mask = mask << 5;
			*celda = *celda | mask;
		}
	}
	
}

#endif // CELDA_H
