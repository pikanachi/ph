#include "gestor_alarmas.h"

#define NUM_EVENTOS 8

Alarma alarmasPendientes[NUM_EVENTOS];

/*
 * Se inicializan con el programa a inv�lidas todas las alarmas y los timers
 */
void ga_inicializar(void) {
		int i;
    temporizador_iniciar();                 //Inicializa el timer 1
		temporizador_empezar();
    temporizador_peridico(50);              //Inicializa el timer0 a para que genere evento Temp_perio cada 50ms
    for (i = 0; i < NUM_EVENTOS; i++) {
        alarmasPendientes[i].esValida = 0;  //Se inicializan las alarmas como no validas
    }
}


/*
 * Una vez que llega un evento de Temp_perio se comprueban las alarmas del gestor.
 * Para cada alarma, si ha llegado el tiempo para irse (en ms) y si es valida
 * encola el evento y si no es peri�dica, la invalida.
 */
void ga_comprobar_alarmas(void) {
		int i;
    for (i = 0; i < NUM_EVENTOS; i++) {
        //Si ha pasado el tiempo para sacarla (/1000 para pasar de us a ms) y la alarma es valida
        if (((clock_gettime() / 1000) >= alarmasPendientes[i].timeToLeave) && alarmasPendientes[i].esValida == 1) {
            Evento e;
            e.ID_evento = alarmasPendientes[i].IDevento;
            cola_guardar_evento(e);

            //Se invalida la alarma si no es peri�dica
            if (!alarmasPendientes[i].esPeriodica) {
							alarmasPendientes[i].esValida = 0;
            } else { //es periodica
							alarmasPendientes[i].timeToLeave = (clock_gettime() / 1000) + alarmasPendientes[i].periodo;
						}
        }
    }
}

/*
 * Operaci�n interna: Crea una nueva alarma a partir de un evento (Set_Alarma) en donde
 * 8bits m�s peso son el evento a generar, 1bit si es peri�dica, 23 restantes el retardo
 * pone su campo esValida a 1.
 */
Alarma crear_alarma(Evento e) {
    Alarma al;
    al.esValida = 1;
    al.IDevento = (e.auxData >> 24) & 0xFF;
    al.esPeriodica = (e.auxData >> 23) & 0x1;
    al.periodo = e.auxData & 0x7FFFFF;
    al.timestamp = e.timestamp;  										// En ms
    al.timeToLeave = al.timestamp + al.periodo;
	
    return al;
}

void set_Alarma(uint8_t id, int periodo, int periodica){
		int retardo;
		Evento eAlarma;
		retardo = periodo & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
		eAlarma.ID_evento = Set_Alarma;
		eAlarma.auxData = id;  							// ID evento a generar
		eAlarma.auxData = eAlarma.auxData << 1;
		if(periodica == 1){
			eAlarma.auxData = eAlarma.auxData | periodica;          	// Es peri�dica
		}
		else{
			eAlarma.auxData = eAlarma.auxData & 0xFFFFFFFE;         // No es peri�dica
		}
		eAlarma.auxData = eAlarma.auxData << 23;
		eAlarma.auxData = eAlarma.auxData | retardo;
		eAlarma.timestamp = clock_gettime() / 1000;
		cola_guardar_evento(eAlarma); 
}

/*
 * Operaci�n interna: Comprueba si tengo alguna alarma pendiente cuyo ID sea el de al.
 * Si lo es y es peri�dica entonces, si el per�odo es 0 la cancela, sino la actualiza.
 */
uint8_t actualizar_alarma(Alarma al) {
		int i;
    uint8_t retVal = 0;
    for (i = 0; i < NUM_EVENTOS; i++) {
        //Si es v�lida, coincide el id y es peri�dica
        if (alarmasPendientes[i].esValida == 1 && alarmasPendientes[i].IDevento == al.IDevento 
            ) {
            //Cancelar alarma
            if (al.periodo == 0) {
                alarmasPendientes[i].esValida = 0;
                retVal = 1;
            //Actualizar periodo
            } else {
							alarmasPendientes[i].timeToLeave = (clock_gettime() / 1000) + al.periodo;
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
		int i;
    //crear nueva alarma y la valida
    Alarma al = crear_alarma(e);

    //Si no se actualiza (es nueva)
    if (!actualizar_alarma(al)) {
        //La insertamos en un slot inv�lido
        for (i = 0; i < NUM_EVENTOS; i++) {
            if (alarmasPendientes[i].esValida == 0) {
                alarmasPendientes[i] = al;
                break;
            }
        }
    }
}

/*
 * Guarda un evento en la cola de temporizador peri�dico
 */
void ga_encolar_evento_temp(void) {
    Evento e;
    e.ID_evento = Temp_perio;
    e.timestamp = clock_gettime() / 1000;   // En ms
    cola_guardar_evento(e);
}
