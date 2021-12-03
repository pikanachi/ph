#ifndef PILA_H
#define PILA_H

#include <stdint.h> // Para los uint

#define MAX_SIZE 10

typedef struct Pila Pila; 

struct Pila {
	char caracteres[MAX_SIZE];
	int index;
};

void crearVacia(void);

/*
 *
 */
void apilar(char c);

/*
 * Si la pila no es vacia devuelve el caracter de la base de la pila 
 * y lo desapila (pila FIFO)
 */
char desapilar(void);

/*
 * Devuelve 1 (verdad) sí y sólo si la pila no tiene ningún caracter.
 * devuelve 0 (falso) en caso contrario.
 */
uint8_t esVacia(void);

#endif
