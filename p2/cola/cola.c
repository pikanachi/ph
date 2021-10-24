#include "cola.h"


//Crea la cola circulasr vacía, es decir, sin ningún elemento
void cola_crear_vacia(uint8_t max, Cola *c) {
    c->numEventos = 0;
    c->maxEventos = max;
    c->prim = NULL;
    c->ult = NULL;
}

void cola_guardar_eventos(Evento *e, Cola *c) {
    Celda celda;
    celda.e = *e;
    if (c->prim == NULL) {
        c->prim = &celda;
        c->ult = &celda;
        celda.ant = &celda;
        celda.sig = &celda;
        c->numEventos ++;
    } else {
        c->ult->sig = &celda;
        celda.ant = c->ult;
        c->ult = &celda;
        if (c->numEventos >= c->maxEventos) {
            c->prim = c->prim->sig;
            delete(c->prim->ant);

            activarFV();

        } else {
            c->numEventos ++;
        }
        c->prim->ant = &celda;
        celda.sig = c->prim;
    }
}

Evento cola_desencola_mas_antiguo(Cola *c) {
    Evento e;
    if (c->prim != NULL) {
        e = c->prim->e;

        //Borrar el primero y actualizar la lista circular
        c->prim = c->prim->sig;
        delete(c->prim->ant);
        c->prim->ant = c->ult;
        c->ult->sig = c->prim;
        c->numEventos--;
    }
    return e;
}

uint8_t cola_es_vacia(Cola *c) {
    return c->numEventos == 0;
}

Evento cola_ultimo(Cola *c) {
    Evento e;
    if (c->prim != NULL) {
        e = c->ult->e;
    }
    return e;
}