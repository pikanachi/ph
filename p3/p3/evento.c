#include "evento.h"

/*
 * Devuelve 1 si el evento tiene id 255 InvalidID.
 * Devuelve 0 en caso contrario.
 */
char es_valido(Evento *e){
	return e->ID_evento != INVALID_ID;
}

Evento el_invalido(void) {
	return eventoInvalido;
}
