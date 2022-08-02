#ifndef _SEM_MODU_H
#define _SEM_MODU_H

#include "../sem.h"
#include "../stack.h"

#define MODU_FINGER 0x4d4f4455

extern int install_modu(bspace *b,bip *ip,int load);

#endif
