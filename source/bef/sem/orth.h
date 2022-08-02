#ifndef _SEM_ORTH_H
#define _SEM_ORTH_H

#include "../sem.h"
#include "../main.h"
#include "../stack.h"
#include "../io.h"
#include "../mem.h"

#define ORTH_FINGER 0x4f525448

extern int install_orth(bspace *b,bip *ip,int load);

#endif
