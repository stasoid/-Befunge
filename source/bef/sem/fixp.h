#ifndef _SEM_FIXP_H
#define _SEM_FIXP_H

#include "../sem.h"
#include "../main.h"
#include "../stack.h"

#define FIXP_FINGER 0x46495850

extern int install_fixp(bspace *b,bip *ip,int load);

#endif
