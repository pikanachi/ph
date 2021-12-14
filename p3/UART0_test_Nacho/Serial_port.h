#include <stdint.h> 

enum {
	ESPERA_CMD = 0,
	ESPERA_NRNUM = 1,
	ESPERA_S = 2,
	ESPERA_T = 3,
	ESPERA_EXC = 4,
	ESPERA_E = 5,
	ESPERA_W = 6,
	ESPERA_COL = 7,
	ESPERA_VAL = 8,
	ESPERA_CHECK = 9
};

void init_serial (void);
int sendchar (int ch) ;
int getchar (void);
void init_serial_byInterrupt(void);
void enviar_string(char *string);
uint8_t ha_terminado(void);
