#ifndef _BEF_MAIN_H
#define _BEF_MAIN_H

#include "../main.h"

extern void bef_reflectip(bspace *b,bip *ip);
extern int bef_inspace(bspace *b,int *pos);
extern void bef_nextpos(bspace *b,bip *ip,int bigskip);
extern void bef_y(bspace *b,bip *ip);
extern void bef_unimplemented(bspace *b,bip *ip,int cell);
extern int bef_checkbreakpoints(bspace *b);
extern void bef_flycheck(bspace *b,bip *ip);
extern int bef_execinstr(bspace *b,bip *ip,int cell);
extern void bef_nextinstr(bspace *b,bip *ip,int cell);
extern void bef_runip(bspace *b,bip *ip);
extern void bef_runprog(bspace *b);

#endif
