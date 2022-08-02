#ifndef _SEM_FRTH_H
#define _SEM_FRTH_H

#include "../sem.h"
#include "../stack.h"
#include "../main.h"

#define FRTH_FINGER 0x46525448

extern int install_frth(bspace *b,bip *ip,int load);

#endif
