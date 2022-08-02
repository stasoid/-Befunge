#ifndef _INTER_USER_H
#define _INTER_USER_H

#include "../main.h"

extern int func_menu_warning(int code,int *block);
extern int func_iconbar_click(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i);
extern void func_tools_click(bspace *b,limpx_icon *i,int buttons);
extern int func_gen_click(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i);
extern int func_newfile_click(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i);
extern int func_menu_selection(int *selections);
extern int func_quit(int code,int *block);
extern int func_key_pressed(limpx_caret_obj *c,int key);
extern int func_break_click(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i);


#endif
