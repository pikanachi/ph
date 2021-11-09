#include "planificador.h"

#define MAX_COLA 32

void plaificador_main(void) {
    //Inicializar gestores
    ga_inicializar();

    //Empezar la cola de eventos
    Evento evento;
    cola_crear_vacia(MAX_COLA);

    while(1) {
        if (!cola_es_vacia()) {
            evento = cola_desencola_mas_antiguo();
            if (es_valido(&evento)) {
                switch(evento.ID_evento) {
                    case Temp_perio : //Evento 0
                        //Ha saltado el temporizador periódico
                        //Se mira en el array de alarmas del gestor si hay alguna alarma que encolar en el planificador
                        ga_comprobar_alarmas();
                        break;
                    case Set_Alarma : //Evento 1
                        //Guardar en el array de alarmas del gestor el evenro
                        ga_guardar_evento(evento);
                        break;
                    case NOMBRE_EVENTO_2 :
                        //Hacer algo
                        break;
                    case NOMBRE_EVENTO_3 :
                        //Hacer algo
                        break;
                    case NOMBRE_EVENTO_4 :
                        //Hacer algo
                        break;
                    case NOMBRE_EVENTO_5 :
                        //Hacer algo
                        break;
                    case NOMBRE_EVENTO_6 :
                        //Hacer algo
                        break;
                    case NOMBRE_EVENTO_7 :
                        //Hacer algo
                        break;
                    case NOMBRE_EVENTO_8 :
                        //Hacer algo
                        break;            
                }
            }
        }
    }
}
