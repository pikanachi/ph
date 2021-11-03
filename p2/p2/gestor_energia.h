#ifndef GESTOR_ENERGIA_H
#define GESTOR_ENERGIA_H

#include "idle_pwdwn.h"

/*
 * Pone al procesador en modo IDE
 */
void ge_modo_IDE(void);

/*
 * Pone al procesador en modo powerdown
 */
void ge_modo_pwdwn(void);

#endif
