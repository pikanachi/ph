#include "planificador.h"
#include "relojes.h"

#define MAX_COLA 32

#define TIME_AL_PUL_TER 100


void planificador_main(void) {
	//Empezar la cola de eventos
	cola_crear_vacia();
	RTC_init();
	//WD_init(10);

	//Inicializar gestores
	GPIO_marcar_entrada(14,16);
	GPIO_marcar_salida(30,2);
	GPIO_marcar_salida(0,14);
	ga_inicializar();
	gIO_inicializar();
	init_serial_byInterrupt(); 
	actualizar_uart();
	gp_inicializar();
	ge_inicializar();
		WD_feed();

	while(1) {
		if(gIO_leer_overflow()){
			while(1);
		}
		if (!cola_es_vacia()) {
			Evento evento;
			evento = cola_desencola_mas_antiguo();
			if (es_valido(&evento)) {
				int retardo;
				Evento eAlarma;
				switch(evento.ID_evento) {
					case Temp_perio :
						//Ha saltado el temporizador periódico
						//Se mira en el array de alarmas del gestor si hay alguna alarma que encolar en el planificador
						ga_comprobar_alarmas();
						break;
					case Set_Alarma :
						//Guardar en el array de alarmas del gestor el evento
						ga_guardar_evento(evento);
						break;
					case Check_Pulsacion_EINT1 :
						//Se mira en el gestor de pulsaciones si sigue pulsada la tecla
						gp_actualizar_estado_EINT1();
						break;
					case Check_Pulsacion_EINT2 :
						//Se comprueba si sigue pulsada EINT2
						gp_actualizar_estado_EINT2();
						break;
					case Pulsacion_EINT1 :
						//Se comprueba si sigue pulsada EINT1
						gIO_escribir_entrada();
						break;
					case Pulsacion_EINT2 :
						//Borrar dato seleccionado de la celda
						gIO_eliminar_valor();
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
					case Terminar:
						//Se reinicia el tablero
						gIO_borrar_tablero();
						//Se encola un evento para chequear la pulsación de fin
						retardo = TIME_AL_PUL_TER & 0x007FFFFF;     					// Asegurarnos que el retardo es de 23bits
						eAlarma.ID_evento = Set_Alarma;
						eAlarma.auxData = Check_Pulsacion_Terminar;  					// ID evento a generar
						eAlarma.auxData = eAlarma.auxData << 1;
						eAlarma.auxData = eAlarma.auxData | 1;         				// Es periódica
						eAlarma.auxData = eAlarma.auxData << 23;
						eAlarma.auxData = eAlarma.auxData | retardo;
						eAlarma.timestamp = temporizador_leer() / 1000;
						cola_guardar_evento(eAlarma); 
						break;
					case Check_Pulsacion_Terminar:
						//Poner al procesador en modo powerDown
						if(gp_leer_pulsacion_1() == 0){ 											// Se ha despulsado
							retardo = 0;
							eAlarma.ID_evento = Set_Alarma;
							eAlarma.auxData = Check_Pulsacion_Terminar;  				// ID evento a generar
							eAlarma.auxData = eAlarma.auxData << 1;
							eAlarma.auxData = eAlarma.auxData | 1;         			// Es periódica
							eAlarma.auxData = eAlarma.auxData << 23;
							eAlarma.auxData = eAlarma.auxData | retardo;
							eAlarma.timestamp = temporizador_leer() / 1000;
							cola_guardar_evento(eAlarma); 
							vaciar_cola();
							ge_modo_pwdwn();
						}
						break;
					case Latido:
						gIO_alternar_latido();
				}
			}
		} else { // Cola vacia
			ge_modo_IDE();
		}
	}
}
