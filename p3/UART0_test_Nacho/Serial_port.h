#include <stdint.h> 

void init_serial (void);
int sendchar (int ch) ;
int getchar (void);
void init_serial_byInterrupt(void);
void enviar_string(char *string);
uint8_t ha_terminado(void);
