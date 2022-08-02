#ifndef _SEM_DIRF_H
#define _SEM_DIRF_H

#include "../sem.h"
#include "../stack.h"
#include "../main.h"

#define DIRF_FINGER 0x44495246

extern int install_dirf(bspace *b,bip *ip,int load);

#endif
