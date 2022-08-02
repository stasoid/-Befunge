#ifndef _BEF_STACK_H
#define _BEF_STACK_H

#include "../main.h"

extern signed long bef_pop(bip *ip);
extern signed long bef_underpop(bip *ip);
extern void bef_push(bip *ip,signed long v);
extern void bef_underpush(bip *ip,signed long v);
extern void bef_popstack(bip *ip);
extern void bef_pushstack(bip *ip);
extern void bef_initstack(bip *ip);
extern void bef_cloneinitstack(bip *ip,bip *clone);
extern void bef_killstack(bip *ip);
extern int bef_stackempty(bip *ip);
extern int bef_stackstackempty(bip *ip);
extern signed long bef_peekstack(bip *ip,int ofs);
extern int bef_countstacks(bip *ip);
extern int bef_stacksize(bip *ip,int s);
extern void bef_pushstring(bip *ip,char *s);

#endif
