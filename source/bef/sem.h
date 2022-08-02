#ifndef _BEF_SEM_H
#define _BEF_SEM_H

#include "../main.h"

extern void bef_loadsem(bspace *b,bip *ip);
extern void bef_unloadsem(bspace *b,bip *ip);
extern void bef_overloadsem(bip *ip,int fing,int cell,int (*instr)(bspace *b,bip *ip,int cell),int load);
extern int bef_callsem(bspace *b,bip *ip,int cell);
extern void bef_initsem(bip *ip);
extern void bef_clonesem(bspace *b,bip *ip,bip *clone);
extern void bef_killsem(bspace *b,bip *ip);
extern void bef_killprogsem(bspace *b);

#endif
