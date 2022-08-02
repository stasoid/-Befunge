#ifndef _BEF_IO_H
#define _BEF_IO_H

#include "../main.h"

extern void bef_cls(bspace *b);
extern void bef_cursor(bspace *b,int x,int y);
extern void bef_cursory(bspace *b,int c);
extern void bef_clearline(bspace *b);
extern void bef_clearscreen(bspace *b);
extern int bef_putchar(bspace *b,int c);
extern int bef_puts(bspace *b,char *s);
extern void bef_backchar(bspace *b);
extern int bef_getchar(bspace *b,bip *ip,int *waitflag);
extern int bef_getint(bspace *b,bip *ip,int base,int *waitflag);
extern int bef_query(bspace *b,bip *ip,int a,int op,int *waitflag);

#endif
