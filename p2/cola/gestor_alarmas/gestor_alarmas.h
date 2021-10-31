#ifndef GESTOR_ALARMAS_H
#define GESTOR_ALARMAS_H

#include "../LPC210X.H"
#include "timers/timers.h"
#include "../evento/evento.h"

typedef struct Alarma Alarma;

struct Alarma {
    uint8_t    esValida;           //indica 1 si es valida 0 no es valida
    uint8_t    IDevento;
    uint8_t    esPeriodica;
    int        periodo;            //ms a disparar la alarma
    int        timestamp;          //timestamp actual en ms
    int        timeToLeave;        //tiempo en el que se ha de sacar la alarma en ms
};

void ga_inicializar();

void ga_guardar_evento(Evento e);

void ga_comprobar_alarmas(void);

#endif