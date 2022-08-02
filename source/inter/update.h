#ifndef _INTER_UPDATE_H
#define _INTER_UPDATE_H

#include "../main.h"

#ifndef LYT
extern void interface_refreshspace(bspace *b,int center);
extern void interface_update_status(bspace *b);
extern void func_update_ipwin(bspace *b);
extern void update_cursor(bspace *b);
extern void interface_scroll(bspace *b);
#else
#define interface_refreshspace(a,b)
#define interface_update_status(b)
#define func_update_ipwin(b)
#define update_cursor(b)
#define interface_scroll(b)
#endif

#endif
