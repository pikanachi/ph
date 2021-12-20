#include "planificador.h"
#include "relojes.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define MAX_COLA 32

#define TIME_AL_PUL_TER 100

char empieza[] = "\n                 SUDOKU                 \n----------------------------------------\nEmpezar partida #NEW!\nTerminar partida #RST!\nConsultar candidatos #fcC!\nIntroducir jugada #fcvs!\nDonde: \nf es la fila(1-9)\nc es la columna(1-9)\nv es el valor a introducir(0 para borrar, 1-9)\ns es el chechsum((f+c+v)%8)\n\nIntroduce tu comando-->";


void planificador_main(void) {
	int fil, col;
	int terminar;
	char time[20];
	char secs[2];
	char msg[320];
	
	//Empezar la cola de eventos
	cola_crear_vacia();
	RTC_init();
	WD_init(20);

	//Inicializar gestores
	ga_inicializar();
	gIO_inicializar();
	gp_inicializar();
	ge_inicializar();
	WD_feed();
	UART_init_serial(); 
	while(1) {	
		if (terminar) {
			//Resetear el rtc
			sprintf(time, "%d", RTC_leer_minutos());
			sprintf(secs, "%d", RTC_leer_segundos());
			strcpy(msg,"\n\n            PARTIDA FINALIZADA          \n----------------------------------------\n");
			strcat(msg, "Tiempo de juego: ");
			strcat(msg, time);
			strcat(msg, " m ");
			strcat(msg, secs);
			strcat(msg, " s\n");
			
			sprintf(time, "%d", tiempo_actualizar());
			strcat(msg, "Tiempo candidatos actualizar: ");
			strcat(msg, time);
			strcat(msg, " ms\n");
			strcat(msg, empieza);	
			UART_enviar_string(msg);
			
			RTC_init();
			reset_timer1();
			WD_init(20);				// Resetear el WD para volver a crear su alarma
			reset_tiempo_actualizar();
			gIO_reset();
			ge_inicializar();
			WD_feed();
			
		} else {
			UART_enviar_string(empieza);
		}
		terminar = 0;
		while(!terminar) {
			if(gIO_leer_overflow()){
				while(1);
			}
			if (!cola_es_vacia()) {
				Evento evento;
				evento = cola_desencola_mas_antiguo();
				if (es_valido(&evento)) {
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
							//gIO_escribir_entrada();
							//CONFIRMAR ENTRADA (Cancelar la alarma de No_Confir_Entrada)
							disable_isr_fiq();
							set_Alarma(No_Confir_Jugada,0,0);
							enable_isr_fiq();
							UART_acaba_jugada();
							UART_enviar_string("Jugada confirmada\nIntroduce tu comando-->\0");
							break;
						case Pulsacion_EINT2 ://<................................................¿QUITAMOS EINT2?
							break;
						case Power_Down:
							ge_modo_pwdwn();
							break;
						case Apagar_Validacion:
							gIO_apagar_validacion();
							break;
						case Terminar:
							//Se reinicia el tablero
							UART_borrar_tablero();
							vaciar_cola();
							ga_reset();
							terminar = 1;	
							break;
						case Latido:
							gIO_alternar_latido();
							break;
						case No_Confir_Jugada:
							UART_cancelar_jugada();
							break;
						case Latido_Validacion:
							gIO_alternar_validacion();
							break;
						case Candidatos:
							col = evento.auxData & 0xFF;
							fil = evento.auxData >> 8;
							UART_enviar_candidatos(fil - 1, col - 1);
							break; 
						case Jugada:
							col = evento.auxData & 0xFF;
							fil = evento.auxData >> 8;
							UART_introducir_jugada(fil - 1, col - 1);
							break; 
						case Feed:
							WD_feed();
							break;
						case Start:
							UART_actualizar("Introduce tu comando-->\0");
					}
				}
			} else { // Cola vacia
				ge_modo_IDE();
			}
		}
	}
}
