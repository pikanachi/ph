#include <LPC210x.H>
#include <string.h>						// para strcpy
#include "Serial_port.h" 

int main (void) {
	int i = 0;
	char string[] = "pikachu";

  init_serial_byInterrupt();      

	enviar_string(string);

	while(i < 1000000) {				//kk de espera que no haremos lo hara el planificador
		i++;
	}
		
	strcpy(string, "hibani");
	enviar_string(string);
	i=0;
	while(1){}
}
