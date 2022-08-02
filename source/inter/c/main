#ifndef _INTERFACE_C
#define _INTERFACE_C

#include "main.h"
#include "../bef/main.h"
#include "update.h"
#include "redraw.h"
#include "user.h"
#include "file.h"
#include "../version.h"

limpx_icon *icon_bar;
limpx_window *win_xfer,*win_proginfo,*win_newfile,*win_break;
limpx_window_header *temp_main,*temp_tools,*temp_io,*temp_ip,*temp_ftools;
limpx_icon_block temp_iconblock;

int open_menu;
bspace *menu_space;

limpx_menu *menu_main;
int menu_main_info,menu_main_quit;
limpx_menu *menu_funge;
int menu_funge_save,menu_funge_98,menu_funge_concurrent,menu_funge_files,menu_funge_script,menu_funge_noprompt,menu_funge_div0,menu_funge_rem0,menu_funge_args,menu_funge_32,menu_funge_goto,menu_funge_warn,menu_funge_break,menu_funge_breakhere,menu_funge_cellvalue,menu_funge_spool,menu_funge_fing,menu_funge_up,menu_funge_inpause;
limpx_menu *menu_div0,*menu_rem0,*menu_args;
int menu_div0_div0,menu_rem0_rem0,menu_args_args;
limpx_menu *menu_goto,*menu_gotopos,*menu_gotoip;
int menu_goto_pos,menu_goto_cursor,menu_goto_ip;
int menu_gotopos_pos;
int menu_gotoip_ip;
limpx_menu *menu_cellvalue;
int menu_cellvalue_value;
limpx_menu *menu_fing;

limpx_event *event_quit,
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

_kernel_swi_regs regs;

void *toolsprites;

int menu_pos[MAX_DIM];

int savetype,menu_fing_size,insertmode;

/*
   Misc functions
*/

void report_error(char *text)
{
#ifdef DEBUG
	fprintf(debuglog,"Reporting error: %s\n",text);
#endif
	LimpX_ReportError(0x600,text,0x711,"Befunge",(char *) NULL,0,(char *) NULL);
}

int func_null_poll()
{
	int cnt;
	bspace *b;
	limpx_window_state s;
	bip *ip;
	/* Find any active programs... */
	b = progs;
	while (b != NULL)
	{
		if (b->flags & PROG_RUNNING)
		{
			if (b->flags & PROG_PAUSED)
				cnt = 1; /* 1 per poll if pause-play */
			else
				cnt = 100; /* 100 per poll if full play */
			ip_refresh = 1;
			while (cnt > 0)
			{
				if (cnt == 1)
					ip_refresh = 1;
				if (bef_checkbreakpoints(b))
					bef_runprog(b);
				else
				{
					cnt = 0; /* Stop straight away */
					ip = b->ips;
					while (ip)
					{
						interface_updatespace(b,ip->pos,(int *) NULL); /* Ensure they get drawn */
						ip = ip->next;
					}
				}
				cnt--;
				ip_refresh = 0;
			}
			func_update_ipwin(b);
			if ((b->ips == NULL) && ((b->flags & PROG_STATUSALERT) == 0))
				interface_statusalert(b,"Program exited normally",0);
		}
		if (b->flags & PROG_INVALIDSTATUS)
		{
			b->flags -= PROG_INVALIDSTATUS;
			interface_update_status(b);
		}
		if (b->flags & PROG_TOOLBARINVALID)
		{
			s = LimpX_GetWindowState(b->win_main);
			func_gen_openwindow(b->win_main,&s);
			b->flags &= ~PROG_TOOLBARINVALID;
		}
		b = b->next;
	}
	return 1;
}

void init_interface(char *infile,char *dstring,int flags,char *args,char *div0,char *rem0,int bits)
{
	limpx_window_header *cur;
	limpx_icon *ic;
	LimpX_Initialise("Befunge");
	/*
	   Load templates
	*/
	LimpX_OpenTemplate("<Befunge$Dir>.Templates");
	cur = LimpX_LoadTemplate("xfer_send");
	win_xfer = LimpX_CreateWindow(cur);
	LimpX_KillTemplate(cur);
	cur = LimpX_LoadTemplate("proginfo");
	win_proginfo = LimpX_CreateWindow(cur);
	LimpX_KillTemplate(cur);
	cur = LimpX_LoadTemplate("new");
	win_newfile = LimpX_CreateWindow(cur);
	LimpX_KillTemplate(cur);
	cur = LimpX_LoadTemplate("break");
	win_break = LimpX_CreateWindow(cur);
	LimpX_KillTemplate(cur);
	temp_main = LimpX_LoadTemplate("main");
	temp_tools = LimpX_LoadTemplate("tools");
	temp_io = LimpX_LoadTemplate("io");
	temp_ip = LimpX_LoadTemplate("ip");
	temp_ftools = LimpX_LoadTemplate("fingtools");
	LimpX_CloseTemplate();
	/*
	   Iconbar icon & menu
	*/
	temp_iconblock.sminx = 0;
	temp_iconblock.sminy = 0;
	temp_iconblock.smaxx = 68;
	temp_iconblock.smaxy = 70;
	temp_iconblock.flags = 0x301A; /* Sprite, H&V centered, click without auto repeat */
	strcpy((char *) temp_iconblock.data,"!Befunge");
	icon_bar = LimpX_NewIconBarIcon(-1,0,&temp_iconblock);

	LimpX_SetIcon_Text(LimpX_IconFromWindow(win_proginfo,0),"Befunge");
	LimpX_SetIcon_Text(LimpX_IconFromWindow(win_proginfo,1),"Interpreting Befunge programs");
	LimpX_SetIcon_Text(LimpX_IconFromWindow(win_proginfo,2),"Jeffrey Lee");
	LimpX_SetIcon_Text(LimpX_IconFromWindow(win_proginfo,3),BEFUNGE_VERSION);

	menu_main = LimpX_NewMenu();
	LimpX_SetMenu_Text(menu_main,"Befunge");
	menu_main_info = LimpX_NewMenuItem(menu_main);
		LimpX_SetMenuItem_IconFlags(menu_main,menu_main_info,0x7000021);
		LimpX_SetMenuItem_Text(menu_main,menu_main_info,"Info");
		LimpX_SetMenuItem_SubWindow(menu_main,menu_main_info,win_proginfo);
	menu_main_quit = LimpX_NewMenuItem(menu_main);
		LimpX_SetMenuItem_IconFlags(menu_main,menu_main_quit,0x7000021);
		LimpX_SetMenuItem_Text(menu_main,menu_main_quit,"Quit");
	LimpX_UpdateMenuBlock(menu_main);
	/*
	   Funge menu
	*/
	/* Structure:
	    [Befunge]
	     Save         > win_xfer
	     Go to        > menu_goto
	     Breakpoints  > win_break
	     Breakpoint here
	     Edit cell    > menu_cellvalue
	     Save output  > win_xfer
	    ---------
	     Script file
	     No input prompts
	     32 bit cells
	     Warnings
	     Enable 98
	    ---------
	     x/0=         > menu_div0
	     x%0=         > menu_rem0
	    ---------
	     Arguments    > menu_args
	     File access
	     Concurrency
	     Up is up
	     Input pauses
	     Dynamic fingerprints > menu_fing */
	menu_funge = LimpX_NewMenu();
	LimpX_SetMenu_Text(menu_funge,"Befunge");
	menu_funge_save = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_save,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_save,"Save");
		LimpX_SetMenuItem_SubWindow(menu_funge,menu_funge_save,win_xfer);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_save,LIMPX_MENUITEM_MESSAGE);
	menu_funge_goto = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_goto,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_goto,"Go to");
		menu_goto = LimpX_NewMenu();
		LimpX_SetMenu_Text(menu_goto,"Go to");
		menu_goto_pos = LimpX_NewMenuItem(menu_goto);
			LimpX_SetMenuItem_IconFlags(menu_goto,menu_goto_pos,0x7000021);
			LimpX_SetMenuItem_Text(menu_goto,menu_goto_pos,"Location");
			menu_gotopos = LimpX_NewMenu();
			LimpX_SetMenu_Text(menu_gotopos,"Location");
			menu_gotopos_pos = LimpX_NewMenuItem(menu_gotopos);
				LimpX_SetMenuItem_IconFlags(menu_gotopos,menu_gotopos_pos,0x7000121);
				LimpX_SetMenuItem_TextLength(menu_gotopos,menu_gotopos_pos,36);
				LimpX_SetMenuItem_ItemFlags(menu_gotopos,menu_gotopos_pos,LIMPX_MENUITEM_WRITEABLE);
			LimpX_UpdateMenuBlock(menu_gotopos);
			LimpX_SetMenuItem_SubMenu(menu_goto,menu_goto_pos,menu_gotopos);
		menu_goto_cursor = LimpX_NewMenuItem(menu_goto);
			LimpX_SetMenuItem_IconFlags(menu_goto,menu_goto_cursor,0x7000021);
			LimpX_SetMenuItem_Text(menu_goto,menu_goto_cursor,"Cursor");
		menu_goto_ip = LimpX_NewMenuItem(menu_goto);
			LimpX_SetMenuItem_IconFlags(menu_goto,menu_goto_ip,0x7000021);
			LimpX_SetMenuItem_Text(menu_goto,menu_goto_ip,"IP");
			menu_gotoip = LimpX_NewMenu();
			LimpX_SetMenu_Text(menu_gotoip,"IP");
			menu_gotoip_ip = LimpX_NewMenuItem(menu_gotoip);
				LimpX_SetMenuItem_IconFlags(menu_gotoip,menu_gotoip_ip,0x7000121);
				LimpX_SetMenuItem_TextLength(menu_gotoip,menu_gotoip_ip,12);
				LimpX_SetMenuItem_ItemFlags(menu_gotoip,menu_gotoip_ip,LIMPX_MENUITEM_WRITEABLE);
			LimpX_UpdateMenuBlock(menu_gotoip);
			LimpX_SetMenuItem_SubMenu(menu_goto,menu_goto_ip,menu_gotoip);
		LimpX_UpdateMenuBlock(menu_goto);
		LimpX_SetMenuItem_SubMenu(menu_funge,menu_funge_goto,menu_goto);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_goto,LIMPX_MENUITEM_MESSAGE);
	menu_funge_break = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_break,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_break,"Breakpoints");
		LimpX_SetMenuItem_SubWindow(menu_funge,menu_funge_break,win_break);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_break,LIMPX_MENUITEM_MESSAGE);
	menu_funge_breakhere = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_breakhere,0x7000121);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_breakhere,"Breakpoint here");
	menu_funge_cellvalue = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_cellvalue,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_cellvalue,"Cell value");
		menu_cellvalue = LimpX_NewMenu();
		LimpX_SetMenu_Text(menu_cellvalue,"Cell value");
		menu_cellvalue_value = LimpX_NewMenuItem(menu_cellvalue);
			LimpX_SetMenuItem_IconFlags(menu_cellvalue,menu_cellvalue_value,0x7000121);
			LimpX_SetMenuItem_TextLength(menu_cellvalue,menu_cellvalue_value,12);
			LimpX_SetMenuItem_ItemFlags(menu_cellvalue,menu_cellvalue_value,LIMPX_MENUITEM_WRITEABLE);
		LimpX_UpdateMenuBlock(menu_cellvalue);
		LimpX_SetMenuItem_SubMenu(menu_funge,menu_funge_cellvalue,menu_cellvalue);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_cellvalue,LIMPX_MENUITEM_MESSAGE);
	menu_funge_spool = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_spool,0x7000121);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_spool,"Save output");
		LimpX_SetMenuItem_SubWindow(menu_funge,menu_funge_spool,win_xfer);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_spool,LIMPX_MENUITEM_MESSAGE+LIMPX_MENUITEM_DOTTED);
	menu_funge_script = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_script,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_script,"Script file");
	menu_funge_noprompt = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_noprompt,0x7000121);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_noprompt,"No input prompts");
	menu_funge_32 = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_32,0x7400121);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_32,"32 bit cells");
	menu_funge_warn = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_warn,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_warn,"Warnings");
	menu_funge_98 = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_98,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_98,"Enable 98"); 
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_98,LIMPX_MENUITEM_DOTTED);
	menu_funge_div0 = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_div0,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_div0,"x/0=");
		menu_div0 = LimpX_NewMenu();
		LimpX_SetMenu_Text(menu_div0,"x/0=");
		menu_div0_div0 = LimpX_NewMenuItem(menu_div0);
			LimpX_SetMenuItem_IconFlags(menu_div0,menu_div0_div0,0x7000121);
			LimpX_SetMenuItem_TextLength(menu_div0,menu_div0_div0,12); 
			LimpX_SetMenuItem_ItemFlags(menu_div0,menu_div0_div0,LIMPX_MENUITEM_WRITEABLE);
		LimpX_UpdateMenuBlock(menu_div0);
		LimpX_SetMenuItem_SubMenu(menu_funge,menu_funge_div0,menu_div0);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_div0,LIMPX_MENUITEM_MESSAGE);
	menu_funge_rem0 = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_rem0,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_rem0,"x%0=");
		menu_rem0 = LimpX_NewMenu();
		LimpX_SetMenu_Text(menu_rem0,"x%0=");
		menu_rem0_rem0 = LimpX_NewMenuItem(menu_rem0);
			LimpX_SetMenuItem_IconFlags(menu_rem0,menu_rem0_rem0,0x7000121);
			LimpX_SetMenuItem_TextLength(menu_rem0,menu_rem0_rem0,12); 
			LimpX_SetMenuItem_ItemFlags(menu_rem0,menu_rem0_rem0,LIMPX_MENUITEM_WRITEABLE);
		LimpX_UpdateMenuBlock(menu_rem0);
		LimpX_SetMenuItem_SubMenu(menu_funge,menu_funge_rem0,menu_rem0);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_rem0,LIMPX_MENUITEM_MESSAGE+LIMPX_MENUITEM_DOTTED);
	menu_funge_args = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_args,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_args,"Arguments");
		menu_args = LimpX_NewMenu();
		LimpX_SetMenu_Text(menu_args,"Arguments");
		menu_args_args = LimpX_NewMenuItem(menu_args);
			LimpX_SetMenuItem_IconFlags(menu_args,menu_args_args,0x7000121);
			LimpX_SetMenuItem_TextLength(menu_args,menu_args_args,256);
			LimpX_SetMenuItem_ItemFlags(menu_args,menu_args_args,LIMPX_MENUITEM_WRITEABLE);
		LimpX_UpdateMenuBlock(menu_args);
		LimpX_SetMenuItem_SubMenu(menu_funge,menu_funge_args,menu_args);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_args,LIMPX_MENUITEM_MESSAGE);
	menu_funge_files = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_files,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_files,"File access"); 
	menu_funge_concurrent = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_concurrent,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_concurrent,"Concurrency");
	menu_funge_up = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_up,0x7000021);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_up,"Up is up");
	menu_funge_inpause = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_inpause,0x7000121);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_inpause,"Input pauses");
	menu_funge_fing = LimpX_NewMenuItem(menu_funge);
		LimpX_SetMenuItem_IconFlags(menu_funge,menu_funge_fing,0x7000121);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_fing,"Dynamic fingerprints");
		menu_fing = LimpX_NewMenu();
		LimpX_SetMenu_Flags(menu_fing,256); /* Indirected */
		LimpX_SetMenu_Text(menu_fing,"Dynamic fingerprints");
		LimpX_UpdateMenuBlock(menu_fing);
		menu_fing_size = 0;
		LimpX_SetMenuItem_SubMenu(menu_funge,menu_funge_fing,menu_fing);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_fing,LIMPX_MENUITEM_MESSAGE);
	LimpX_UpdateMenuBlock(menu_funge);
	/*
	   Events
	*/
	open_menu = 0;
	menu_space = (bspace *) NULL;

	event_iconbar_click = LimpX_NewEvent_Mouse_Click(func_iconbar_click,(limpx_window *) NULL,icon_bar);
	event_menu_selection = LimpX_NewEvent_Menu_Selection(func_menu_selection);
	event_quit = LimpX_NewEvent_User_Message(func_quit,LIMPX_USERMSG_QUIT);
	event_gen_openwindow = LimpX_NewEvent_Open_Window(func_gen_openwindow,(limpx_window *) NULL);
	event_close_window = LimpX_NewEvent_Close_Window(func_close_window,(limpx_window *) NULL);
	event_gen_redraw = LimpX_NewEvent_Redraw_Window(func_gen_redraw,(limpx_window *) NULL);
	event_newfile_click = LimpX_NewEvent_Mouse_Click(func_newfile_click,win_newfile,(limpx_icon *) NULL);
	event_load_handler = LimpX_NewEvent_User_Message(func_load_handler,LIMPX_USERMSG_DATA_LOAD);
	event_null_poll = LimpX_NewEvent_Null_Code(func_null_poll);
	event_gen_click = LimpX_NewEvent_Mouse_Click(func_gen_click,(limpx_window *) NULL,(limpx_icon *) NULL);
	event_key_pressed = LimpX_NewEvent_Key_Pressed(func_key_pressed,(limpx_window *) NULL,(limpx_icon *) NULL);
	event_menu_warning = LimpX_NewEvent_User_Message(func_menu_warning,LIMPX_USERMSG_MENU_WARNING);
	event_save_drag = LimpX_NewEvent_Mouse_Click(func_save_drag,win_xfer,LimpX_IconFromWindow(win_xfer,2));
	event_save_msg_ok = LimpX_NewEvent_User_Message(func_save_msg_ok,LIMPX_USERMSG_DATA_SAVE_ACK);
	event_save_ok = LimpX_NewEvent_Mouse_Click(func_save_ok,win_xfer,LimpX_IconFromWindow(win_xfer,0));
	event_break_click = LimpX_NewEvent_Mouse_Click(func_break_click,win_break,(limpx_icon *) NULL);
	toolsprites = OSSpr_LoadFile("<Befunge$Dir>.Sprites");
	/*
	   Set up newfile window
	*/
	ic = LimpX_IconFromWindow(win_newfile,4);
	if (flags & PROG_98)
		LimpX_SetIcon_Flags(ic,LimpX_GetIcon_Flags(ic) | LIMPX_ICONFLAG_SELECTED);
	ic = LimpX_IconFromWindow(win_newfile,8);
	if (flags & PROG_CON)
		LimpX_SetIcon_Flags(ic,LimpX_GetIcon_Flags(ic) | LIMPX_ICONFLAG_SELECTED);
	ic = LimpX_IconFromWindow(win_newfile,5);
	if (flags & PROG_FS)
		LimpX_SetIcon_Flags(ic,LimpX_GetIcon_Flags(ic) | LIMPX_ICONFLAG_SELECTED);
	ic = LimpX_IconFromWindow(win_newfile,9);
	if (flags & PROG_SCRIPT)
		LimpX_SetIcon_Flags(ic,LimpX_GetIcon_Flags(ic) | LIMPX_ICONFLAG_SELECTED);
	ic = LimpX_IconFromWindow(win_newfile,10);
	if (flags & PROG_NOPROMPT)
		LimpX_SetIcon_Flags(ic,LimpX_GetIcon_Flags(ic) | LIMPX_ICONFLAG_SELECTED);
	if (div0)
		LimpX_SetIcon_Text(LimpX_IconFromWindow(win_newfile,15),div0);
	if (rem0)
		LimpX_SetIcon_Text(LimpX_IconFromWindow(win_newfile,17),rem0);
	ic = LimpX_IconFromWindow(win_newfile,19);
	if (flags & PROG_WARN_UNIMPLEMENTED)
		LimpX_SetIcon_Flags(ic,LimpX_GetIcon_Flags(ic) | LIMPX_ICONFLAG_SELECTED);
	ic = LimpX_IconFromWindow(win_newfile,18);
	if (bits == 32)
		LimpX_SetIcon_Flags(ic,LimpX_GetIcon_Flags(ic) | LIMPX_ICONFLAG_SELECTED);
	LimpX_SetIcon_Text(LimpX_IconFromWindow(win_newfile,3),dstring);
	LimpX_SetIcon_Text(LimpX_IconFromWindow(win_newfile,12),args);
	/*
	   Load file if needed
	*/
	if (infile)
	{
		LimpX_SetIcon_Text(LimpX_IconFromWindow(win_newfile,1),infile);
		func_newfile_click((limpx_mouse_obj *) NULL,win_newfile,LimpX_IconFromWindow(win_newfile,7)); /* Create false click */
	}
	insertmode = 0;
}

void interface_poll()
{
	limpx_pollblock *p;
	while (LimpX_GetAbortFlag() == 0)
	{
		p = LimpX_Poll(0,NULL);
#ifdef DEBUG
		fprintf(debuglog,"Poll returns %d:\n",p->code);
		fprintf(debuglog,"%d,%X,%d,%d,%d\n",p->block[0],p->block[1],p->block[2],p->block[3],p->block[4]);
#endif
	}
}

void finish_interface()
{
#ifdef DEBUG
	fprintf(debuglog,"finish_interface()\n");
#endif
	/* Shut everything down... */
	LimpX_KillAll();
	LimpX_CloseDown();
}

#endif
