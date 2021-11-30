#ifndef GESTOR_ALARMAS_H
#define GESTOR_ALARMAS_H

#include <LPC210x.H>
#include "timers.h"
#include "evento.h"
#include "cola.h"
#include "softI.h"

typedef struct Alarma Alarma;

struct Alarma {
    uint8_t    esValida;           //indica 1 si es valida 0 no es valida
    uint8_t    IDevento;
    uint8_t    esPeriodica;
    int        periodo;            //ms a disparar la alarma
    int        timestamp;          //timestamp actual en ms
    int        timeToLeave;        //tiempo en el que se ha de sacar la alarma en ms
};


void set_Alarma(uint8_t id, int periodo, int periodica);

/*
 * Se inicializan con el programa a inválidas todas las alarmas y los timers
 */
void ga_inicializar(void);


/*
 * Una vez que llega un evento de Temp_perio se comprueban las alarmas del gestor.
 * Para cada alarma, si ha llegado el tiempo para irse (en ms) y si es valida
 * encola el evento y si no es periódica, la invalida.
 */
void ga_guardar_evento(Evento e);


/*
 * Comprueba si el evento que le llega (Set_Alarma) ha de actualizar alguna alarma del
 * sistema y si no ha de actualizarla la mete dentro de las alarmas pendientes
 */
void ga_comprobar_alarmas(void);


/*
 * Guarda un evento en la cola de temporizador periódico
 */
void ga_encolar_evento_temp(void);

#endif
