#include "planificador.h"

#define MAX_COLA 32

void planificador_main(void) {
		int i;
		//Empezar la cola de eventos
		cola_crear_vacia();

		//Inicializar gestores
		GPIO_marcar_entrada(14,16);
		GPIO_marcar_salida(30,2);
		GPIO_marcar_salida(0,14);
		ga_inicializar();
		gIO_inicializar();
    gp_inicializar();
		ge_inicializar();
		candidatos_actualizar_c(cuadricula_C_C);

    while(1) {
				if(gIO_leer_overflow()){
					while(1);
				}
        if (!cola_es_vacia()) {
					  Evento evento;
            evento = cola_desencola_mas_antiguo();
            if (es_valido(&evento)) {
                switch(evento.ID_evento) {
                    case Temp_perio : //Evento 0
                        //Ha saltado el temporizador periódico
                        //Se mira en el array de alarmas del gestor si hay alguna alarma que encolar en el planificador
                        ga_comprobar_alarmas();
                        break;
                    case Set_Alarma : //Evento 1
                        //Guardar en el array de alarmas del gestor el evento
                        ga_guardar_evento(evento);
                        break;
                    case Check_Pulsacion_EINT1 : //Evento 2
                        //Se mira en el gestor de pulsaciones si sigue pulsada la tecla
                        gp_actualizar_estado_EINT1();
                        break;
                    case Check_Pulsacion_EINT2 : //Evento 3
                        gp_actualizar_estado_EINT2();
                        break;
                    case Pulsacion_EINT1 :
												gIO_escribir_entrada();
                        break;
                    case Pulsacion_EINT2 :
												i = i + 1;
                        //Hacer algo
												//Borrar dato seleccionado
                        break;
										case Power_Down:
												//Poner al procesador en modo powerDown
												ge_modo_pwdwn();
												break;
										case Check_Entrada:
												//Poner al procesador en modo powerDown
												gIO_check_entrada();
												break;
										case Apagar_Validacion:
												//Poner al procesador en modo powerDown
												gIO_apagar_validacion();
												break;
                }
            }
        } else { // Cola vacia
            ge_modo_IDE();
        }
    }
}
