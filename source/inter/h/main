#ifndef _INTER_MAIN_H
#define _INTER_MAIN_H

#include "kernel.h"
#include "swis.h"

#include "../main.h"
#include "/WoumInclude:lib/osspr.h"

extern limpx_icon *icon_bar;
extern limpx_window *win_xfer,*win_proginfo,*win_newfile,*win_break;
extern limpx_window_header *temp_main,*temp_tools,*temp_io,*temp_ip,*temp_ftools;
extern limpx_icon_block temp_iconblock;

extern int open_menu;
extern bspace *menu_space;

extern limpx_menu *menu_main;
extern int menu_main_info,menu_main_quit;
extern limpx_menu *menu_funge;
extern int menu_funge_save,menu_funge_98,menu_funge_concurrent,menu_funge_files,menu_funge_script,menu_funge_noprompt,menu_funge_div0,menu_funge_rem0,menu_funge_args,menu_funge_32,menu_funge_goto,menu_funge_warn,menu_funge_break,menu_funge_breakhere,menu_funge_cellvalue,menu_funge_spool,menu_funge_fing,menu_funge_up,menu_funge_inpause;
extern limpx_menu *menu_div0,*menu_rem0,*menu_args;
extern int menu_div0_div0,menu_rem0_rem0,menu_args_args;
extern limpx_menu *menu_goto,*menu_gotopos,*menu_gotoip;
extern int menu_goto_pos,menu_goto_cursor,menu_goto_ip;
extern int menu_gotopos_pos;
extern int menu_gotoip_ip;
extern limpx_menu *menu_cellvalue;
extern int menu_cellvalue_value;
extern limpx_menu *menu_fing;
extern int menu_fing_size;

extern limpx_event *event_quit,
            *event_iconbar_click,
            *event_menu_selection,
            *event_gen_openwindow,
            *event_close_window,
            *event_load_handler,
            *event_newfile_click,
            *event_gen_redraw,
            *event_null_poll,
            *event_gen_click,
            *event_key_pressed,
            *event_menu_warning,
            *event_save_drag,
            *event_save_msg_ok,
            *event_drag_box,
            *event_save_ok,
            *event_break_click;

extern _kernel_swi_regs regs;

extern void *toolsprites;

extern int menu_pos[MAX_DIM];

extern int savetype,insertmode;

#define SAVETYPE_NORM 1
#define SAVETYPE_IO 2
#define SAVETYPE_SPOOL 3

// Text size in OS units
#define XSCALE 16
#define YSCALE 32

// Insert mode bitflags
#define INSERT_CODE 1
#define INSERT_STRING 2
#define INSERT_SUPER 4
#define INSERT_FORCE 8
#define INSERT_LOCK 16

extern void report_error(char *text);
extern int func_null_poll();

#endif
