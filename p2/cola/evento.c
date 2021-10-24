#include "evento.h"

void evento_crear(uint8_t ID, uint32_t aux, uint32_t ts, Evento *e) {
    e->ID_evento = ID;
    e->auxData = aux;
    e->timestamp = ts;
}