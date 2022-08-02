#ifndef _SEM_ROMA_H
#define _SEM_ROMA_H

#include "../sem.h"
#include "../stack.h"

#define ROMA_FINGER 0x524f4d41

extern int install_roma(bspace *b,bip *ip,int load);

#endif
