#ifndef PARA_FRAMEUPDATE_H
#define PARA_FRAMEUPDATE_H

#include "../main.h"

// Run a frame once
void sys_gameTickRun ();

// Handle system events and populate the keyboard state array
void sys_processInputEvents();

#endif //PARA_FRAMEUPDATE_H
