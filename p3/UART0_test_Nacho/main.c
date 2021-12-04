#include <LPC210x.H>
#include <string.h>						// para strcpy
#include "Serial_port.h" 

int main (void) {
	char string[] = "pikachu";
	//Inicializar la UART0 por interrupcion
  init_serial_byInterrupt();      

	enviar_string(string);	
	strcpy(string, "hibani");
	while (!ha_terminado());
	enviar_string(string);

	while(1){}
}
