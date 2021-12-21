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
			vaciar_cola();
			ga_reset();
			ge_modo_pwdwn();
			UART_reset();
			//Resetear el rtc

			
			RTC_reset();
			reset_timer1();
			WD_init(20);				// Resetear el WD para volver a crear su alarma
			reset_tiempo_actualizar();
			gIO_reset();
			ge_inicializar();
			WD_feed();
			terminar = 0;
			UART_enviar_string(empieza);
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
						case Pulsacion_EINT1 :
							//CONFIRMAR ENTRADA (Cancelar la alarma de No_Confir_Entrada)
							if(en_jugada()){
								disable_isr_fiq();
								set_Alarma(No_Confir_Jugada,0,0);
								enable_isr_fiq();
								UART_acaba_jugada();
								UART_enviar_string("Jugada confirmada\nIntroduce tu comando-->\0");
							}
							break;
						case Power_Down:
							ge_modo_pwdwn();
							break;
						case Apagar_Validacion:
							gIO_apagar_validacion();
							break;
						case Terminar:
							//Se reinicia el tablero
							if(evento.auxData != 1){
								strcpy(msg,"\n\n            PARTIDA FINALIZADA          \n----------------------------------------\n");
							} else{
								strcpy(msg,"\n\n                HAS GANADO              \n----------------------------------------\n");
							}
							UART_borrar_tablero();
							set_Alarma(Check_Terminado_PWDOWN, 150, 1);
							sprintf(time, "%d", RTC_leer_minutos());
							sprintf(secs, "%d", RTC_leer_segundos());
							
							strcat(msg, "Tiempo de juego: ");
							strcat(msg, time);
							strcat(msg, " m ");
							strcat(msg, secs);
							strcat(msg, " s\n");
							
							sprintf(time, "%d", tiempo_actualizar());
							strcat(msg, "Tiempo candidatos actualizar: ");
							strcat(msg, time);
							strcat(msg, " ms\n");
							UART_enviar_string(msg);
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
							RTC_init();
							UART_actualizar("Introduce tu comando-->\0");
							break;
						case Check_Terminado_PWDOWN:
							if(UART_ha_terminado() && (gp_leer_pulsacion_1() == 0)){
								terminar = 1;
							}
							break;
						
					}
				}
			} else { // Cola vacia
				ge_modo_IDE();
			}
		}
	}
}
