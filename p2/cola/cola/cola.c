#include "cola.h"
//#include <stdio.h>

static Cola c;  //static para que no la lie al compilar con otras globales (otra var con el mismo nombre en otro c)

/*
 * Crea la cola circular vacía, es decir, sin Evento.
 */
void cola_crear_vacia() {
    c.numEventos = 0;
    c.indProxEv = 0;
    c.indPrimEv = 0;
}

/*
void actualizarFV() {
    printf("FlagV\n");
}
*/

/*
 * Si numEventos < maxEventos devuelve la cola circular resultante de añadir a c
 * el Evento e. Si no, devuelve la cola resultante de añadir a c el Evento e, eliminando
 * el Evento más antiguo de la cola, activando el flag V (Overflow).
 */
void cola_guardar_evento(Evento e) {
    c.eventos[c.indProxEv] = e;

    if (!cola_es_vacia() && c.indPrimEv == c.indProxEv) {   //Overflow
        c.indPrimEv = (c.indPrimEv + 1) % MAX_EVENTOS;
        actualizarFV();
    }else{
        c.numEventos++;
    }
    c.indProxEv = (c.indProxEv + 1) % MAX_EVENTOS;
}


/* 
 * Si numEventos > 0, devuelve el Evento más antiguo de la cola (el primero) y devuelve la
 * cola circular resultante de eliminar de la cola dicho Evento. En caso contrario, devuelve
 * un Evento inválido.
 */
Evento cola_desencola_mas_antiguo() {
    Evento e = eventoInvalido;
    if(!cola_es_vacia()) {
        e = c.eventos[c.indPrimEv];
        c.indPrimEv = (c.indPrimEv + 1) % MAX_EVENTOS;
        c.numEventos--;
    }
    return e;
}


/*
 * Devuelve 1 (verdad) sí y sólo si la cola c no tiene ningún Evento.
 * devuelve 0 (falso) en caso contrario.
 */
uint8_t cola_es_vacia() {
    return c.numEventos == 0;
}


/*
 * Si numEventos > 0, devuelve el Evento más nuevo encolado en la cola (el último). En caso contrario, devuelve
 * un evento inválido.
 */
Evento cola_ultimo() {
    Evento e = eventoInvalido;
    if (!cola_es_vacia()) {
        if (c.indProxEv-1 < 0) {  //Es el último
            e = c.eventos[MAX_EVENTOS-1];
        } else {
            e = c.eventos[c.indProxEv-1];
        }
    }
    return e;
}

/*
void imp_cola() {
    for (int i = 0; i<MAX_EVENTOS; i++) {
        printf("%d | ", c.eventos[i].ID_evento);
    }
    printf("\n");
}
*/
