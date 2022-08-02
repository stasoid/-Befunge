#ifndef _SEM_REFC_H
#define _SEM_REFC_H

#include "../sem.h"
#include "../main.h"
#include "../stack.h"

#define REFC_FINGER 0x52454643

extern int install_refc(bspace *b,bip *ip,int load);
extern void killprog_refc(bspace *b);

#endif
