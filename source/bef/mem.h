#ifndef _BEF_MEM_H
#define _BEF_MEM_H

#include "../main.h"

extern void bef_initcells(bspace *b);
extern void bef_killcells(bspace *b);
extern signed long bef_readspace(bspace *b,int *pos,int *by);
extern void bef_setspace(bspace *b,int *pos,int *by,signed long v);
extern unsigned long bef_getmemspace(bspace *b,int *pos,int indim,int dir);

#endif
