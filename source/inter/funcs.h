#ifndef _INTER_FUNCS_H
#define _INTER_FUNCS_H

#include "../main.h"

extern void func_do_save(char *name);
extern void func_fiddle_newfile_window(char *fname,char *args);
extern void open_funge_menu(bspace *b,limpx_mouse_obj *m);
extern void deselect_tool(bspace *b,int i);
extern void select_tool(bspace *b,int i);
extern void interface_goto(bspace *b,int *pos);
extern void parse_coord_string(bspace *b,char *n,int *pos);

#endif
