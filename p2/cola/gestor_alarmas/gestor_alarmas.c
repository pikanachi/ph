#include "gestor_alarmas.h"

#define NUM_EVENTOS 8

Alarma alarmasPendientes[NUM_EVENTOS];

/*
 * Se inicializan con el programa a inválidas todas las alarmas y los timers
 */
void ga_inicializar() {
    temporizador_iniciar();                 //Inicializa el timer 1
    temporizador_peridico(50);              //¿CADA CUANTO? Inicializa el timer0 <---------------------------
    for (int i = 0; i<NUM_EVENTOS; i++) {
        alarmasPendientes[i].esValida = 0;  //Se inicializan las alarmas como no validas
    }
}

/*
 * Una vez que llega un evento de Temp_perio se comprueban las alarmas del gestor.
 * Para cada alarma, si ha llegado el tiempo para irse (en ms) y si es valida
 * encola el evento y si no es periódica, la invalida.
 */
void ga_comprobar_alarmas(void) {
    for (int i = 0; i < NUM_EVENTOS; i++) {
        //Si ha pasado el tiempo para sacarla (/1000 para pasar de us a ms) y la alarma es valida
        if ((alarmasPendientes[i].timeToLeave >= temporizador_leer()/1000) && alarmasPendientes[i].esValida == 1) {
            Evento e;
            e.ID_evento = alarmasPendientes[i].IDevento;
            cola_guardar_evento(e);

            //Se invalida la alarma si no es periódica
            if (!alarmasPendientes[i].esPeriodica) {
                alarmasPendientes[i].esValida = 0;
            }
        }
    }
}

/*
 * Operación interna: Crea una nueva alarma a partir de un evento (Set_Alarma) en donde
 * 8bits más peso son el evento a generar, 1bit si es periódica, 23 restantes el retardo
 * pone su campo esValida a 1.
 */
Alarma crear_alarma(Evento e) {
    Alarma al;
    al.esValida = 1;
    al.IDevento = (e.auxData >> 24) & 0xFF;
    al.esPeriodica = (e.auxData >> 23) & 0x1;
    al.periodo = e.auxData & 0x7FFFFF;
    al.timestamp = temporizador_leer() / 1000;
    al.timeToLeave = al.timestamp + al.periodo;
    return al;
}

/*
 * Operación interna: Comprueba si tengo alguna alarma pendiente cuyo ID sea el de al.
 * Si lo es y es periódica entonces, si el período es 0 la cancela, sino la actualiza.
 */
uint8_t actualizar_alarma(Alarma al) {
    uint8_t retVal = 0;
    for (uint8_t i = 0; i < NUM_EVENTOS; i++) {
        //Si es válida, coincide el id y es periódica
        if (alarmasPendientes[i].esValida == 1 && alarmasPendientes[i].IDevento == al.IDevento 
            && alarmasPendientes[i].esPeriodica) {
            //Cancelar alarma
            if (al.periodo == 0) {
                alarmasPendientes[i].esValida = 0;
                retVal = 1;
            //Actualizar periodo
            } else {
                alarmasPendientes[i].periodo = al.periodo;
                retVal = 1;
            }
        }
    }
    return retVal;
}

/*
 * Comprueba si el evento que le llega (Set_Alarma) ha de actualizar alguna alarma del
 * sistema y si no ha de actualizarla la mete dentro de las alarmas pendientes
 */
void ga_guardar_evento(Evento e) {
    //crear nueva alarma y la valida
    Alarma al = crear_alarma(e);

    //Si no se actualiza (es nueva)
    if (!actualizar_alarma(al)) {
        //La insertamos en un slot inválido
        for (uint8_t i = 0; i < NUM_EVENTOS; i++) {
            if (alarmasPendientes[i].esValida == 0) {
                alarmasPendientes[i] = al;
                break;
            }
        }
    }
}
