#ifndef _INTER_USER_C
#define _INTER_USER_C

#include "user.h"
#include "main.h"
#include "funcs.h"
#include "../bef/ip.h"
#include "../bef/io.h"
#include "../bef/main.h"
#include "../bef/mem.h"
#include "update.h"
#include "../bef/prog.h"
#include "redraw.h"

/*
   User interactions
*/

int func_menu_warning(int code,int *block)
{
	char tstr[36],*n;
	int tmp,tmp2;
	limpx_icon *i;
	limpx_window_state s;
	bspace *b;
	if (open_menu == MENU_FUNGE)
	{
		if (block[8] == menu_funge_save)
		{
			savetype = SAVETYPE_NORM;
			LimpX_SetIcon_Text(LimpX_IconFromWindow(win_xfer,1),menu_space->fname);
			LimpX_SetIcon_TextLength(LimpX_IconFromWindow(win_xfer,1),256);
			s = LimpX_GetWindowState(win_xfer);
			s.vminy = s.vmaxy-180;
			LimpX_OpenWindowAt(win_xfer,s);
			LimpX_CreateSubMenu((limpx_menu *) NULL,win_xfer,block[6],block[7]);
			LimpX_UpdateCaret();
			return 1;
		}
		else if (block[5] == LimpX_HandleFromMenu(menu_div0))
		{
			if (menu_space->flags & PROG_QUERYDIV0)
				LimpX_SetMenuItem_Text(menu_div0,menu_div0_div0,"");
			else
			{
				sprintf(tstr,"%d",menu_space->div0);
				LimpX_SetMenuItem_Text(menu_div0,menu_div0_div0,tstr);
			}
			LimpX_SetMenuItem_TextLength(menu_div0,menu_div0_div0,12); 
			LimpX_UpdateMenuBlock(menu_div0);
			LimpX_CreateSubMenu(menu_div0,(limpx_window *) NULL,block[6],block[7]);
		}
		else if (block[5] == LimpX_HandleFromMenu(menu_rem0))
		{
			if (menu_space->flags & PROG_QUERYREM0)
				LimpX_SetMenuItem_Text(menu_rem0,menu_rem0_rem0,"");
			else
			{
				sprintf(tstr,"%d",menu_space->rem0);
				LimpX_SetMenuItem_Text(menu_rem0,menu_rem0_rem0,tstr);
			}
			LimpX_SetMenuItem_TextLength(menu_rem0,menu_rem0_rem0,12); 
			LimpX_UpdateMenuBlock(menu_rem0);
			LimpX_CreateSubMenu(menu_rem0,(limpx_window *) NULL,block[6],block[7]);
		}
		else if (block[5] == LimpX_HandleFromMenu(menu_args))
		{
			LimpX_SetMenuItem_Text(menu_args,menu_args_args,menu_space->args);
			LimpX_SetMenuItem_TextLength(menu_args,menu_args_args,256); 
			LimpX_UpdateMenuBlock(menu_args);
			LimpX_CreateSubMenu(menu_args,(limpx_window *) NULL,block[6],block[7]);
		}
		else if (block[5] == LimpX_HandleFromMenu(menu_goto))
		{
			LimpX_SetMenuItem_Text(menu_gotopos,menu_gotopos_pos,"");
			LimpX_SetMenuItem_TextLength(menu_gotopos,menu_gotopos_pos,36);
			LimpX_UpdateMenuBlock(menu_gotopos);
			if (menu_space->ips) /* Fill in ID of 1st IP */
				sprintf(tstr,"%d",menu_space->ips->id);
			else
				tstr[0] = 0;
			LimpX_SetMenuItem_Text(menu_gotoip,menu_gotoip_ip,tstr);
			LimpX_SetMenuItem_TextLength(menu_gotoip,menu_gotoip_ip,12);
			LimpX_UpdateMenuBlock(menu_gotoip);
			LimpX_CreateSubMenu(menu_goto,(limpx_window *) NULL,block[6],block[7]);
		}
		else if (block[5] == LimpX_HandleFromWindow(win_break))
		{
			/* Fill in the window... */
			for (tmp=0;tmp<NUM_BREAKPOINTS;tmp++)
			{
				i = LimpX_IconFromWindow(win_break,2+tmp*2);
				if (menu_space->breakflags & (1 << tmp))
					LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_SELECTED);
				else
					LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_SELECTED);
				n = tstr;
				n += sprintf(n,"%d",menu_space->breaks[tmp][0]);
				for (tmp2=1;tmp2<menu_space->dims;tmp2++)
					n += sprintf(n,",%d",menu_space->breaks[tmp][tmp2]);
				i = LimpX_IconFromWindow(win_break,3+tmp*2); 
				LimpX_SetIcon_Text(i,tstr);
				LimpX_SetIcon_TextLength(i,36);
			}
			LimpX_CreateSubMenu((limpx_menu *) NULL,win_break,block[6],block[7]);
			LimpX_UpdateCaret();
			return 1;
		}
		else if (block[5] == LimpX_HandleFromMenu(menu_cellvalue))
		{
			sprintf(tstr,"%d",(int) bef_readspace(menu_space,menu_pos,(int *) NULL));
			LimpX_SetMenuItem_Text(menu_cellvalue,menu_cellvalue_value,tstr);
			LimpX_SetMenuItem_TextLength(menu_cellvalue,menu_cellvalue_value,12);
			LimpX_UpdateMenuBlock(menu_cellvalue);
			LimpX_CreateSubMenu(menu_cellvalue,(limpx_window *) NULL,block[6],block[7]);
		}
		else if (block[8] == menu_funge_spool)
		{
			savetype = SAVETYPE_IO;
			LimpX_SetIcon_Text(LimpX_IconFromWindow(win_xfer,1),"Output");
			LimpX_SetIcon_TextLength(LimpX_IconFromWindow(win_xfer,1),256);
			s = LimpX_GetWindowState(win_xfer);
			s.vminy = s.vmaxy-240;
			LimpX_OpenWindowAt(win_xfer,s);
			LimpX_CreateSubMenu((limpx_menu *) NULL,win_xfer,block[6],block[7]);
			LimpX_UpdateCaret();
			return 1;
		}
		else if (block[5] == LimpX_HandleFromMenu(menu_fing))
		{
			while (menu_fing_size > 0)
			{
				menu_fing_size--;
				LimpX_KillMenuItem(menu_fing,0);
			}
			/* Now find all fingerprint spaces for menu_space */
			b = progs;
			while (b)
			{
				if ((b->flags & PROG_FINGERPRINT) && (b->haunted == menu_space))
				{
					LimpX_NewMenuItem(menu_fing);
					LimpX_SetMenuItem_IconFlags(menu_fing,menu_fing_size,0x7000021);
					tmp2 = 0;
					for (tmp=0;tmp<4;tmp++)
					{
						tstr[tmp] = b->fing >> (24-(8*tmp));
						if ((tstr[tmp] < 32) || (tstr[tmp] == 127))
							tmp2 = 1;
					}
					if (tmp2)
						sprintf(tstr,"0x%08x",b->fing);
					else
						tstr[4] = 0;
					LimpX_SetMenuItem_Text(menu_fing,menu_fing_size,tstr);
					menu_fing_size++;
				}
				b = b->next;
			}
			LimpX_UpdateMenuBlock(menu_fing);
			LimpX_CreateSubMenu(menu_fing,(limpx_window *) NULL,block[6],block[7]);
			return 1;
		}
	}
	return 0;
}

int func_iconbar_click(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i)
{
#ifdef DEBUG
	fprintf(debuglog,"Iconbar click! %d,%d, %X,%X\n",m->x,m->y,(int) w,(int) i);
#endif
	/* Check buttons */
	if (m->buttons == 2)
	{
		/* Open menu */
		LimpX_CreateMenu(menu_main,m->x-64,m->y+(44*2));
		open_menu = MENU_MAIN;
		return 1;
	}
	if (m->buttons == 4)
	{
		/* Create new file... */
		func_fiddle_newfile_window("","");
		LimpX_OpenWindow(win_newfile);
		return 1;
	}
	return 0;
}

void func_tools_click(bspace *b,limpx_icon *i,int buttons)
{
	int ic,state;
	limpx_window_state wstate;
	bip *ip;
	ic = LimpX_HandleFromIcon(i);
	if (b->flags & PROG_FINGERPRINT)
	{
		if (ic == 0) /* Fingerprint windows have subset of tools */
			ic = 8;
		else if (ic == 1)
			ic = 6;
		else
			return;
	}
	state = LimpX_GetIcon_Flags(i) & LIMPX_ICONFLAG_SELECTED;
	switch (ic)
	{
		case 0: /* Play */
			if (state)
			{ /* Go to play & remove pause state if select used */
				b->flags |= PROG_RUNNING;
				if ((b->flags & PROG_PAUSED) == 0)
				{
					ip = bef_newip(b,(bip *) NULL);
					ip->delta[0] = 1;
				}
				if (buttons & 4) /* Select */
				{
					b->flags &= ~PROG_PAUSED;
					deselect_tool(b,1);
				}
				deselect_tool(b,2);
			}
			else
			{ /* Remove play state, go to stop if needed */
				if ((b->flags & PROG_PAUSED) == 0)
				{
					select_tool(b,2);
					while (b->ips != NULL)
						bef_killip(b,b->ips);
					bef_cls(b);
				}
				b->flags &= ~PROG_RUNNING;
			}
			return;
		case 1: /* Pause */
			if (state)
			{ /* Go to pause & remove play state if select used */
				b->flags |= PROG_PAUSED;
				if ((b->flags & PROG_RUNNING) == 0)
				{
					ip = bef_newip(b,(bip *) NULL);
					ip->delta[0] = 1;
				}
				if (buttons & 4) /* Select */
				{
					b->flags &= ~PROG_RUNNING;
					deselect_tool(b,0);
				}
				deselect_tool(b,2);
			}
			else
			{ /* Remove pause state, go to stop if needed */
				if ((b->flags & PROG_RUNNING) == 0)
				{
					select_tool(b,2);
					while (b->ips != NULL)
						bef_killip(b,b->ips);
					bef_cls(b);
				}
				b->flags &= ~PROG_PAUSED;
			}
			return;
		case 2: /* Stop */
			if (state)
			{
				b->flags &= ~(PROG_RUNNING + PROG_PAUSED);
				while (b->ips != NULL)
					bef_killip(b,b->ips);
				bef_cls(b);
				/* Deselect other icons */
				deselect_tool(b,0);
				deselect_tool(b,1);
				b->flags |= PROG_INVALIDSTATUS;
			}
			return;
		case 3: /* IO window */
			wstate = LimpX_GetWindowState(b->win_io);
			wstate.behind = -1; /* Ontop */
			LimpX_OpenWindowAt(b->win_io,wstate);
			return;
		case 4: /* Up */
			if (b->dims < 3)
				return;
			b->cursor[2] += UP_DIR(b);
			if ((b->cursor[2] < 0) && (b->sizes[2] != 0))
				b->cursor[2] += b->sizes[2];
			if ((b->cursor[2] >= (int) b->sizes[2]) && (b->sizes[2] != 0))
				b->cursor[2] -= b->sizes[2];
			LimpX_ForceRedraw(b->win_main,0,-32000,32000,0);
			return;
		case 5: /* Down */
			if (b->dims < 3)
				return;
			b->cursor[2] += DOWN_DIR(b);
			if ((b->cursor[2] < 0) && (b->sizes[2] != 0))
				b->cursor[2] += b->sizes[2];
			if ((b->cursor[2] >= (int) b->sizes[2]) && (b->sizes[2] != 0))
				b->cursor[2] -= b->sizes[2];
			LimpX_ForceRedraw(b->win_main,0,-32000,32000,0);
			return;
		case 6: /* Status line */
			/* Clear alert if set */
			if (b->flags & PROG_STATUSALERT)
			{
				b->flags -= PROG_STATUSALERT;
				LimpX_SetIcon_Validation(i,"R2"); /* Normal slab */
				interface_update_status(b); /* Reset */
			}
			return;
		case 7: /* Step */
			deselect_tool(b,0); /* Play */
			deselect_tool(b,2); /* Stop */
			select_tool(b,1); /* Pause */
			/* Work out whether to spawn an IP */
			if ((b->flags & (PROG_RUNNING + PROG_PAUSED)) == 0)
			{
				ip = bef_newip(b,(bip *) NULL);
				ip->delta[0] = 1;
				state = 0; /* Don't run yet */
			}
			else
				state = 1; /* Run */
			b->flags = b->flags | PROG_PAUSED;
			b->flags -= b->flags & PROG_RUNNING;
			ip_refresh = 1;
			if (bef_checkbreakpoints(b) && (state))
				bef_runprog(b); /* Run it for a cycle */
			func_update_ipwin(b);
			if ((b->ips == NULL) && ((b->flags & PROG_STATUSALERT) == 0))
				interface_statusalert(b,"Program exited normally",0);
			return;
		case 8: /* IP window */
			wstate = LimpX_GetWindowState(b->win_ip);
			wstate.behind = -1;
			LimpX_OpenWindowAt(b->win_ip,wstate);
			func_update_ipwin(b);
			/* Nasty hack to get it to resize */
			LimpX_OpenWindow(b->win_ip);
			return;
		case 9: /* Reset button */
			b->flags -= b->flags & (PROG_RUNNING + PROG_PAUSED);
			bef_resetprog(b);
			/* Reload file */
			if (strcmp(b->fname,"Untitled") != 0) /* Only if the file is from disk */
				if (load_file(b,b->fname,(int *) 0,0,(int *) 0) == EOF)
					report_error("Unable to load file");
			select_tool(b,2);
			deselect_tool(b,0);
			deselect_tool(b,1);
			interface_refreshspace(b,0);
			func_update_ipwin(b);
			b->flags |= PROG_INVALIDSTATUS;
			return;
	}
}

int func_gen_click(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i)
{
	int win;
	bspace *b;
	int x,y;
	limpx_window_state s;
	b = progs;
	win = -1;
	while (win == -1)
	{
		if (b == NULL)
			return 1;
		if ((b->win_main == w) && (b->win_main))
			win = 1;
		else if ((b->win_tools == w) && (b->win_tools))
			win = 2;
		else if ((b->win_io == w) && (b->win_io))
			win = 3;
		else if ((b->win_ip == w) && (b->win_ip))
			win = 4;
		else
			b = b->next;
	}
	switch (win)
	{
		case 1: /* Main window */
			/* Decode mouse pos... */
			interface_updatespace(b,b->cursor,0);
			s = LimpX_GetWindowState(w);
			x = m->x + (s.scrollx - s.vminx);
			y = m->y + (s.scrolly - s.vmaxy);
			if (m->buttons == 4)
				LimpX_SetCaretPosition(b->win_main,(limpx_icon *) NULL,x,y,-1,-1);
			if (x < 0)
				x -= XSCALE;
			if (y > 0)
				y += YSCALE;
			x = x/XSCALE;
			y = -y/YSCALE;
			if (m->buttons == 4)
			{
				b->cursor[0] = x;
				if (b->dims > 1)
					b->cursor[1] = y;
				interface_updatespace(b,b->cursor,0);
			}
			else if (m->buttons == 2)
			{
				menu_pos[0] = x;
				menu_pos[1] = y;
				for (x=2;x<b->dims;x++)
					menu_pos[x] = b->cursor[x];
				open_funge_menu(b,m);
			}
			break;
		case 2: /* Tools window */
			if (m->buttons == 2)
				open_funge_menu(b,m);
			else if ((m->buttons == 4) || (m->buttons == 1)) /* Ignore double click, etc. */
				func_tools_click(b,i,m->buttons);
			break;
		case 3: /* IO window */
			if (m->buttons == 4)
				LimpX_SetCaretPosition(b->win_io,(limpx_icon *) NULL,0,0,-1,-1);
			else if (m->buttons == 2)
				open_funge_menu(b,m);
			break;
		case 4: /* IP window */
			if (m->buttons == 2)
				open_funge_menu(b,m);
			break;
	}
	return 1;
}

int func_newfile_click(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i)
{
	char *n;
	int dims,tmp;
	int sizes[MAX_DIM];
	int bits,flags;
	int minx,miny,maxx,maxy;
	limpx_window_state s;
	limpx_icon_block ib;
	if (i == LimpX_IconFromWindow(win_newfile,6))
	{
		LimpX_CloseWindow(w);
		return 1;
	}
	if (i == LimpX_IconFromWindow(win_newfile,7))
	{
		/* Create new level... */
		bspace *b;
		/* Get info */
		bits = 8;
		flags = 0;
		if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,4)) & LIMPX_ICONFLAG_SELECTED)
			flags |= PROG_98;
		if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,8)) & LIMPX_ICONFLAG_SELECTED)
			flags |= PROG_CON;
		if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,5)) & LIMPX_ICONFLAG_SELECTED)
			flags |= PROG_FS;
		if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,9)) & LIMPX_ICONFLAG_SELECTED)
			flags |= PROG_SCRIPT;
		if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,10)) & LIMPX_ICONFLAG_SELECTED)
			flags |= PROG_NOPROMPT;
		if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,19)) & LIMPX_ICONFLAG_SELECTED)
			flags |= PROG_WARN_UNIMPLEMENTED;
		if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,20)) & LIMPX_ICONFLAG_SELECTED)
			flags |= PROG_UP_IS_UP;
		if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,21)) & LIMPX_ICONFLAG_SELECTED)
			flags |= PROG_INPUT_PAUSES;
		if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,18)) & LIMPX_ICONFLAG_SELECTED)
			bits = 32;
		/* Now work out size... */
		n = LimpX_GetIcon_Text(LimpX_IconFromWindow(win_newfile,3));
		tmp = 0; /* unlimited size */
		sizes[0] = sizes[1] = 1; /* Dummy sizes to make sure the window is OK */
		dims = 1;
		while ((*n != 0) && (dims <= MAX_DIM))
		{
			if ((*n >= '0') && (*n <= '9')) /* number */
			{
				tmp = 10*tmp + (*n)-'0';
			}
			else if (*n == '*') /* unlimited size */
				tmp = 0;
			else if (*n == ',') /* next def. */
			{
				sizes[dims-1] = tmp;
				dims++;
				tmp = 0;
			}
			n++;
		}
		/* Also the last entry... */
		sizes[dims-1] = tmp;
		for (tmp=dims;tmp<MAX_DIM;tmp++)
			sizes[tmp] = 1; /* Zero other values */
		b = bef_newprog(dims,sizes,bits); /* Create bspace */
		/* Check div0/rem0 */
		n = LimpX_GetIcon_Text(LimpX_IconFromWindow(win_newfile,15));
		if (*n)
			b->div0 = atoi(n);
		else
			b->flags |= PROG_QUERYDIV0;
		n = LimpX_GetIcon_Text(LimpX_IconFromWindow(win_newfile,17));
		if (*n)
			b->rem0 = atoi(n);
		else
			b->flags |= PROG_QUERYREM0;
		/* Check whether there is a file to load */
		n = LimpX_GetIcon_Text(LimpX_IconFromWindow(win_newfile,1));
		if (strlen(n) <= 0)
			n = "Untitled";
		else
			if (load_file(b,n,(int *) 0,0,(int *) 0) == EOF)
				report_error("Unable to load file");
		strcpy(b->fname,n);
		b->flags |= flags;
		strcpy(b->args,LimpX_GetIcon_Text(LimpX_IconFromWindow(win_newfile,12)));
		/* Now open window */
		b->win_main = LimpX_CreateWindow(temp_main);
		LimpX_SetWindow_Title(b->win_main,n);
		if (b->sizes[0] > 0)
		{
			minx = 0;
			maxx = b->sizes[0]*XSCALE;
		}
		else
		{
			/* Infinite size */
			/* Use +/- 32000 */
			minx = -32000;
			maxx = 32000;
		}
		if (b->sizes[1] > 0)
		{
			miny = b->sizes[1]*-YSCALE;
			maxy = 56;
		}
		else
		{
			miny = -32000;
			maxy = 32056; /* Don't forget the toolbar! */
		}
		LimpX_SetExtent(b->win_main,minx,miny,maxx,maxy);
		b->win_tools = LimpX_CreateWindow(temp_tools);
		LimpX_SetWindow_SpriteBlock(b->win_tools,(int) toolsprites);
		b->win_io = LimpX_CreateWindow(temp_io);
		LimpX_SetWindow_Title(b->win_io,n);
		b->win_ip = LimpX_CreateWindow(temp_ip);
		LimpX_SetWindow_Title(b->win_ip,n);
		s = LimpX_GetWindowState(b->win_main);
		s.behind = -1;
		s.scrollx = 0;
		s.scrolly = 56; /* Toolbar height */
		func_gen_openwindow(b->win_main,&s); /* Cheat and use this code to open main & tool windows */
		LimpX_CloseWindow(win_newfile);
		LimpX_SetCaretPosition(b->win_main,(limpx_icon *) NULL,0,0,-1,-1);
		return 1;
	}
	if (i == LimpX_IconFromWindow(win_newfile,4))
	{
		/* Toggle visibility of other icons */
		func_fiddle_newfile_window((char *) NULL,(char *) NULL);
		LimpX_GetIconState(LimpX_IconFromWindow(win_newfile,2),&ib);
		LimpX_ForceRedraw(win_newfile,ib.sminx,ib.sminy,ib.smaxx,ib.smaxy);
		return 1;
	}
	if ((i == LimpX_IconFromWindow(win_newfile,3)) && (m->buttons == 1))
	{
		/* Right-click on dims box */
		/* Cycle through 80,25 -> *,* -> *,*,* -> * */
		n = LimpX_GetIcon_Text(i);
		if (strcmp(n,"*,*") == 0)
			n = "*,*,*";
		else if (strcmp(n,"*,*,*") == 0)
			n = "*";
		else if (strcmp(n,"*") == 0)
			n = "80,25";
		else /* 80,25 or something else */
			n = "*,*";
		LimpX_SetIcon_Text(i,n);
		LimpX_GetIconState(i,&ib);
		LimpX_ForceRedraw(win_newfile,ib.sminx,ib.sminy,ib.smaxx,ib.smaxy);
		LimpX_UpdateCaret();
		return 1;
	}
	return 0;
}

int func_menu_selection(int *selections)
{
	limpx_mouse_obj m;
	char *n;
	int tmp,tpos[MAX_DIM],tmp2;
	bip *ip;
	bspace *b;
	limpx_window_state s;
	switch (open_menu)
	{
	case MENU_MAIN:
		if (selections[0] == menu_main_quit)
		{
#ifdef DEBUG
			fprintf(debuglog,"Quit chosen from menu\n");
#endif
			LimpX_SetAbortFlag(1);
		}
		break;
	case MENU_FUNGE:
		if ((menu_space->flags & PROG_FINGERPRINT) == 0)
		{
			if (selections[0] == menu_funge_98)
				menu_space->flags ^= PROG_98; /* Won't change bittyness though :( */
			if (selections[0] == menu_funge_concurrent)
				menu_space->flags ^= PROG_CON;
			if (selections[0] == menu_funge_files)
				menu_space->flags ^= PROG_FS;
			if (selections[0] == menu_funge_script)
				menu_space->flags ^= PROG_SCRIPT;
			if (selections[0] == menu_funge_noprompt)
				menu_space->flags ^= PROG_NOPROMPT;
			if (selections[0] == menu_funge_warn)
				menu_space->flags ^= PROG_WARN_UNIMPLEMENTED;
			if (selections[0] == menu_funge_up)
				menu_space->flags ^= PROG_UP_IS_UP;
			if (selections[0] == menu_funge_inpause)
				menu_space->flags ^= PROG_INPUT_PAUSES;
			if (selections[0] == menu_funge_div0)
			{
				n = LimpX_GetMenuItem_Text(menu_div0,menu_div0_div0);
				if (*n)
				{
					menu_space->flags &= ~PROG_QUERYDIV0;
					menu_space->div0 = atoi(n);
				}
				else
					menu_space->flags |= PROG_QUERYDIV0;
			}
			if (selections[0] == menu_funge_rem0)
			{
				n = LimpX_GetMenuItem_Text(menu_rem0,menu_rem0_rem0);
				if (*n)
				{
					menu_space->flags &= ~PROG_QUERYREM0;
					menu_space->rem0 = atoi(n);
				}
				else
					menu_space->flags |= PROG_QUERYREM0;
			}
			if (selections[0] == menu_funge_args)
				strcpy(menu_space->args,LimpX_GetMenuItem_Text(menu_args,menu_args_args));
			if (selections[0] == menu_funge_spool)
			{
				if (menu_space->spool)
					fclose(menu_space->spool);
				menu_space->spool = (FILE *) NULL;
			}
		}
		if (selections[0] == menu_funge_goto)
		{
			if (selections[1] == menu_goto_pos)
			{
				parse_coord_string(menu_space,LimpX_GetMenuItem_Text(menu_gotopos,menu_gotopos_pos),tpos);
				interface_goto(menu_space,tpos);
			}
			else if (selections[1] == menu_goto_cursor)
				interface_goto(menu_space,menu_space->cursor);
			else if (selections[1] == menu_goto_ip)
			{
				tmp = atoi(LimpX_GetMenuItem_Text(menu_gotoip,menu_gotoip_ip));
				ip = menu_space->ips;
				while ((ip) && (ip->id != tmp))
					ip = ip->next;
				if (ip)
					interface_goto(menu_space,ip->pos);
			}
		}
		if (selections[0] == menu_funge_breakhere)
		{
			tmp2 = -1; /* Not found */
			for (tmp=0;tmp<NUM_BREAKPOINTS;tmp++)
				if (menu_space->breakflags & (1 << tmp))
				{
					if (memcmp(menu_space->breaks[tmp],menu_pos,menu_space->dims*4) == 0)
					{
						tmp2 = tmp;
						break;
					}
				}
				else
					tmp2 = tmp;
			if (tmp2 != -1)
			{
				memcpy(menu_space->breaks[tmp2],menu_pos,menu_space->dims*4);
				menu_space->breakflags ^= 1 << tmp2; /* Toggle flag */
			}
			interface_updatespace(menu_space,menu_pos,(int *) NULL);
		}
		if (selections[0] == menu_funge_cellvalue)
		{
			n = LimpX_GetMenuItem_Text(menu_cellvalue,menu_cellvalue_value);
			if (*n)
			{
				sscanf(n,"%i",&tmp);
				bef_setspace(menu_space,menu_pos,(int *) NULL,tmp);
			}
		}
		if ((selections[0] == menu_funge_fing) && (selections[1] != -1) && (menu_fing_size > 0))
		{
			n = LimpX_GetMenuItem_Text(menu_fing,selections[1]);
			if (strlen(n) == 4)
			{
				tmp = 0;
				for (tmp2=0;tmp2<4;tmp2++)
					tmp = tmp*256 + n[tmp2];
			}
			else
				sscanf(n,"0x%8x",&tmp);
			b = progs;
			while (b)
			{
				if ((b->flags & PROG_FINGERPRINT) && (b->haunted == menu_space) && (b->fing == tmp))
				{
					/* Found it */
					if (b->win_main == NULL)
					{
						/* Create windows */
						b->win_main = LimpX_CreateWindow(temp_main);
						LimpX_SetWindow_Title(b->win_main,b->fname);
						LimpX_SetExtent(b->win_main,-32000,-32000,32000,32056);
						b->win_tools = LimpX_CreateWindow(temp_ftools);
						LimpX_SetWindow_SpriteBlock(b->win_tools,(int) toolsprites);
						b->win_ip = LimpX_CreateWindow(temp_ip);
						LimpX_SetWindow_Title(b->win_ip,b->fname);
						s = LimpX_GetWindowState(b->win_main);
						s.behind = -1;
						s.scrollx = 0;
						s.scrolly = 56; /* Toolbar height */
					}
					else
						s = LimpX_GetWindowState(b->win_main);
					func_gen_openwindow(b->win_main,&s);
				}
				b = b->next;
			}
		}
		break;
	}
	LimpX_GetPointerInfo(&m);
	if (m.buttons & 1)
	{
		if (open_menu == MENU_FUNGE)
			open_funge_menu(menu_space,(limpx_mouse_obj *) NULL);
		else
			LimpX_CreateMenu((limpx_menu *) NULL,0,0);
	}
	return 1;
}

int func_quit(int code,int *block)
{
#ifdef DEBUG
	fprintf(debuglog,"Quit message received\n");
#endif
	LimpX_SetAbortFlag(1);
	return 1;
}

static void insert_text(bspace *b,int *pos,int *delta,int recurse,int c)
{
	/* Insert char c at pos, shuffling text if required */
	int t,tt;
	int pos2[MAX_DIM],delta2[MAX_DIM];
	recurse++;
	if (recurse > 100)
		return; /* Abort */
	t = bef_readspace(b,pos,(int *) NULL);
	if (((insertmode & (INSERT_STRING | INSERT_FORCE)) == 0) && (t == 32))
	{
		bef_setspace(b,pos,(int *) NULL,c); /* No shuffle required */
		return;
	}
	for (tt=0;tt<b->dims;tt++)
		pos2[tt] = pos[tt] + delta[tt];
	/* Else may need to shuffle */
	if (insertmode & INSERT_FORCE)
	{
		insertmode -= INSERT_FORCE;
		insert_text(b,pos2,delta,recurse,t); /* Shuffle this char */
		bef_setspace(b,pos,(int *) NULL,c); /* And overwrite it */
		return;
	}
	else if (insertmode & INSERT_STRING)
	{
		if (t == 34)
			insertmode -= INSERT_STRING; /* Now leaving string */
		insert_text(b,pos2,delta,recurse,t); /* Shuffle this char */
		bef_setspace(b,pos,(int *) NULL,c); /* And overwrite it */
		return;
	}
	else if (insertmode & INSERT_SUPER)
	{
		/* Super code insert */
		if (t == 34)
		{
			insertmode |= INSERT_STRING; /* Enter string */
			insert_text(b,pos2,delta,recurse,t); /* Shuffle this char */
			bef_setspace(b,pos,(int *) NULL,c); /* And overwrite it */
			return;
		}
		else if ((t == 39) || (t == 's'))
		{
			insertmode |= INSERT_FORCE;
			insert_text(b,pos2,delta,recurse,t); /* Shuffle this char */
			bef_setspace(b,pos,(int *) NULL,c); /* And overwrite it */
			return;
		}
		else if (t == '^')
		{
			for (t=0;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[1] = -1;
			pos2[1]--;
		}
		else if (t == 'v')
		{
			for (t=0;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[1] = 1;
			pos2[1]++;
		}
		else if (t == '<')
		{
			for (t=1;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[0] = -1;
			pos2[0]--;
		}
		else if (t == '>')
		{
			for (t=1;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[0] = 1;
			pos2[0]++;
		}
		else if (t == 'h')
		{
			for (t=0;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[2] = UP_DIR(b);
			pos2[2]--;
		}
		else if (t == 'l')
		{
			for (t=0;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[2] = DOWN_DIR(b);
			pos2[2]++;
		}
		else if (t == '[')
		{
			delta2[0] = delta[1];
			delta2[1] = -delta[0];
			for (t=0;t<b->dims;t++)
			{
				if (t > 1)
					delta2[t] = delta[t];
				pos2[t]=pos[t]+delta2[t];
			}
		}
		else if (t == ']')
		{
			delta2[0] = -delta[1];
			delta2[1] = delta[0];
			for (t=0;t<b->dims;t++)
			{
				if (t > 1)
					delta2[t] = delta[t];
				pos2[t]=pos[t]+delta2[t];
			}
		}
		else
		{
			/* We are ontop of a regular code char */
			insert_text(b,pos2,delta,recurse,t); /* Shuffle char we're ontop of */
			bef_setspace(b,pos,(int *) NULL,c); /* And overwrite it */
			return;
		}
		/* ... else we are ontop of a direction change control */
		insert_text(b,pos2,delta2,recurse,c); /* So try writing c at the location it points to */
		return;
	}
	else
	{
		/* Plain code insert mode */
		if (t == 34)
		{
			insertmode |= INSERT_STRING; /* Enter string */
			insert_text(b,pos2,delta,recurse,t); /* Shuffle this char */
			bef_setspace(b,pos,(int *) NULL,c); /* And overwrite it */
			return;
		}
		else if ((t == 39) || (t == 's'))
		{
			insertmode |= INSERT_FORCE;
			insert_text(b,pos2,delta,recurse,t); /* Shuffle this char */
			bef_setspace(b,pos,(int *) NULL,c); /* And overwrite it */
			return;
		}
		else
		{
			/* We are ontop of a regular code char */
			insert_text(b,pos2,delta,recurse,t); /* Shuffle char we're ontop of */
			bef_setspace(b,pos,(int *) NULL,c); /* And overwrite it */
			return;
		}
	}
}

static int delete_text(bspace *b,int *pos,int *delta,int recurse)
{
	/* Delete the char at 'pos', replacing it with the value at pos+delta. Return the original value (so that calls further up know what to place at their own location) */
	int pos2[MAX_DIM],t,tt,delta2[MAX_DIM];
	recurse++;
	if (recurse > 100)
		return 32; /* Abort */
	t = bef_readspace(b,pos,(int *) NULL);
	if ((t == 32) && ((insertmode & (INSERT_STRING | INSERT_FORCE)) == 0))
		return 32; /* No shuffling required */
	for (tt=0;tt<b->dims;tt++)
		pos2[tt] = pos[tt] + delta[tt];
	tt = bef_readspace(b,pos2,(int *) NULL);
	if (insertmode & INSERT_FORCE)
	{
		insertmode -= INSERT_FORCE;
		tt = delete_text(b,pos2,delta,recurse); /* Shuffle next char */
		bef_setspace(b,pos,(int *) NULL,tt); /* And overwrite this one */
		return t;
	}
	else if (insertmode & INSERT_STRING)
	{
		if (t == 34)
			insertmode -= INSERT_STRING;
		tt = delete_text(b,pos2,delta,recurse); /* Shuffle next char */
		bef_setspace(b,pos,(int *) NULL,tt); /* And overwrite current */
		return t;
	}
	else if (insertmode & INSERT_SUPER)
	{
		/* Super code insert */
		if (t == 34)
		{
			insertmode |= INSERT_STRING; /* Enter string */
			tt = delete_text(b,pos2,delta,recurse); /* Shuffle next char */
			bef_setspace(b,pos,(int *) NULL,tt); /* And overwrite current */
			return t;
		}
		else if ((t == 39) || (t == 's'))
		{
			insertmode |= INSERT_FORCE;
			tt = delete_text(b,pos2,delta,recurse); /* Shuffle next char */
			bef_setspace(b,pos,(int *) NULL,tt); /* And overwrite current */
			return t;
		}
		else if (t == '^')
		{
			for (t=0;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[1] = -1;
			pos2[1]--;
		}
		else if (t == 'v')
		{
			for (t=0;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[1] = 1;
			pos2[1]++;
		}
		else if (t == '<')
		{
			for (t=1;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[0] = -1;
			pos2[0]--;
		}
		else if (t == '>')
		{
			for (t=1;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[0] = 1;
			pos2[0]++;
		}
		else if (t == 'h')
		{
			for (t=0;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[2] = UP_DIR(b);
			pos2[2]--;
		}
		else if (t == 'l')
		{
			for (t=0;t<b->dims;t++)
			{
				pos2[t] = pos[t];
				delta2[t] = 0;
			}
			delta2[2] = DOWN_DIR(b);
			pos2[2]++;
		}
		else if (t == '[')
		{
			delta2[0] = delta[1];
			delta2[1] = -delta[0];
			for (t=0;t<b->dims;t++)
			{
				if (t > 1)
					delta2[t] = delta[t];
				pos2[t]=pos[t]+delta2[t];
			}
		}
		else if (t == ']')
		{
			delta2[0] = -delta[1];
			delta2[1] = delta[0];
			for (t=0;t<b->dims;t++)
			{
				if (t > 1)
					delta2[t] = delta[t];
				pos2[t]=pos[t]+delta2[t];
			}
		}
		else
		{
			/* We are ontop of a regular code char */
			tt = delete_text(b,pos2,delta,recurse); /* Shuffle next char */
			bef_setspace(b,pos,(int *) NULL,tt); /* And overwrite current */
			return t;
		}
		/* ... else we are ontop of a direction change control */
		return delete_text(b,pos2,delta2,recurse); /* So try deleting the char it points to */
	}
	else
	{
		/* Plain code insert mode */
		if (t == 34)
		{
			insertmode |= INSERT_STRING; /* Enter string */
			tt = delete_text(b,pos2,delta,recurse); /* Shuffle next char */
			bef_setspace(b,pos,(int *) NULL,tt); /* And overwrite current */
			return t;
		}
		else if ((t == 39) || (t == 's'))
		{
			insertmode |= INSERT_FORCE;
			tt = delete_text(b,pos2,delta,recurse); /* Shuffle next char */
			bef_setspace(b,pos,(int *) NULL,tt); /* And overwrite current */
			return t;
		}
		else
		{
			/* We are ontop of a regular code char */
			tt = delete_text(b,pos2,delta,recurse); /* Shuffle next char */
			bef_setspace(b,pos,(int *) NULL,tt); /* And overwrite current */
			return t;
		}
	}
}

static void type_key(bspace *b,int key)
{
	/* Enter 'key' into b's fungespace at cursor */
	int delta[MAX_DIM],t,omode;
	for (t=0;t<b->dims;t++)
		delta[t] = 0;
	if (b->cdir >= 128)
		delta[b->cdir-128] = -1;
	else
		delta[b->cdir] = 1;
	if (insertmode)
	{
		omode = insertmode;
		insert_text(b,b->cursor,delta,0,key);
		insertmode = omode;
	}
	else
		bef_setspace(b,b->cursor,0,key);
	if (insertmode & INSERT_LOCK)
		return; /* Don't move cursor */
	if (b->cdir >= 128)
	{
		b->cursor[b->cdir-128]--;
		if ((b->sizes[b->cdir & 127]) && (b->cursor[b->cdir & 127] < 0))
			b->cursor[b->cdir & 127] += b->sizes[b->cdir & 127];
	}
	else
	{
		b->cursor[b->cdir]++;
		if ((b->sizes[b->cdir]) && (b->cursor[b->cdir] >= (int) b->sizes[b->cdir]))
			b->cursor[b->cdir] -= b->sizes[b->cdir];
	}
}

static void delete_key(bspace *b)
{
	/* Delete what's under cursor */
	int delta[MAX_DIM],t,omode;
	for (t=0;t<b->dims;t++)
		delta[t] = 0;
	if (b->cdir >= 128)
		delta[b->cdir-128] = -1;
	else
		delta[b->cdir] = 1;
	if (insertmode)
	{
		omode = insertmode;
		delete_text(b,b->cursor,delta,0);
		insertmode = omode;
		return;
	}
	bef_setspace(b,b->cursor,0,32);
	if (b->cdir >= 128)
	{
		b->cursor[b->cdir-128]++;
		if ((b->sizes[b->cdir & 127]) && (b->cursor[b->cdir & 127] >= (int) b->sizes[b->cdir & 127]))
			b->cursor[b->cdir & 127] -= b->sizes[b->cdir & 127];
	}
	else
	{
		b->cursor[b->cdir]--;
		if ((b->sizes[b->cdir]) && (b->cursor[b->cdir] < 0))
			b->cursor[b->cdir] += b->sizes[b->cdir];
	}
}

int func_key_pressed(limpx_caret_obj *c,int key)
{
	bspace *b,*bs;
	char *s;
	int interested;
	b = progs;
	interested = 0; /* If this is still zero later on, then pass the key back to the WIMP */
	while (b != NULL)
	{
		if ((b->win_io) && (LimpX_HandleFromWindow(b->win_io) == c->window))
		{
			if ((key > 0) && (key < 256))
				interested = 1;
			if (b->input_ip != NULL)
			{
				if ((key == 0x7F) || (key == 0x08))
				{
					/* Delete/backspace */
					if (b->textbuf_chars)
					{
						bef_backchar(b);
						s = b->text_input;
						while (*(s+1))
							s++;
						*s = 0;
						b->textbuf_chars--;
					}
				}
				else if (b->textbuf_chars < 255)
				{
					if (key == 13)
						key = 10;
					if ((key == 10) || (key > 31) || (key < 256))
					{
						bef_putchar(b,key);
						s = b->text_input;
						while (*s)
							s++;
						*(s++) = key;
						*s = 0;
						b->textbuf_chars++;
					}
				}
			}
		}
		else if ((b->win_main) && (LimpX_HandleFromWindow(b->win_main) == c->window))
		{
			/* Process key... */
			interface_updatespace(b,b->cursor,0);
			if (key == 0x18D) /* right */
			{
				b->cursor[0]++;
				if ((b->sizes[0]) && (b->cursor[0] >= (int) b->sizes[0]))
					b->cursor[0] -= b->sizes[0];
				interested = 1;
			}
			else if (key == 0x1AD) /* ctrl+right */
			{
				b->cdir = 0;
				interested = 1;
			}
			else if (key == 0x18C) /* left */
			{
				b->cursor[0]--;
				if ((b->sizes[0]) && (b->cursor[0] < 0))
					b->cursor[0] += b->sizes[0];
				interested = 1;
			}
			else if (key == 0x1AC) /* ctrl+left */
			{
				b->cdir = 128;
				interested = 1;
			}
			else if (b->dims > 1)
			{
				if (key == 0x18E) /* down */
				{
					b->cursor[1]++;
					if ((b->sizes[1]) && (b->cursor[1] >= (int) b->sizes[1]))
						b->cursor[1] -= b->sizes[1];
					interested = 1;
				}
				else if (key == 0x1AE) /* ctrl+down */
				{
					b->cdir = 1;
					interested = 1;
				}
				else if (key == 0x18F) /* up */
				{
					b->cursor[1]--;
					if ((b->sizes[1]) && (b->cursor[1] < 0))
						b->cursor[1] += b->sizes[1];
					interested = 1;
				}
				else if (key == 0x1AF) /* ctrl+up */
				{
					b->cdir = 129;
					interested = 1;
				}
				else if (b->dims > 2)
				{
					if (key == 0x19E) /* page down */
					{
						b->cursor[2] += DOWN_DIR(b);
						if ((b->sizes[2]) && (b->cursor[2] < 0))
							b->cursor[2] += b->sizes[2];
						if ((b->sizes[2]) && (b->cursor[2] >= (int) b->sizes[2]))
							b->cursor[2] -= b->sizes[2];
						interface_refreshspace(b,0);
						interested = 1;
					}
					else if (key == 0x1BE) /* ctrl+page down */
					{
						if (DOWN_DIR(b) == 1)
							b->cdir = 2;
						else
							b->cdir = 130;
						interested = 1;
					}
					else if (key == 0x19F) /* page up */
					{
						b->cursor[2] += UP_DIR(b);
						if ((b->sizes[2]) && (b->cursor[2] < 0))
							b->cursor[2] += b->sizes[2];
						if ((b->sizes[2]) && (b->cursor[2] >= (int) b->sizes[2]))
							b->cursor[2] -= b->sizes[2];
						interface_refreshspace(b,0);
						interested = 1;
					}
					else if (key == 0x1BF) /* ctrl+page up */
					{
						if (UP_DIR(b) == -1)
							b->cdir = 130;
						else
							b->cdir = 2;
						interested = 1;
					}
				}
			}
			if ((key == 0x008) || (key == 0x7F)) /* delete */
			{
				delete_key(b);
				interested = 1;
			}
			else if ((key > 31) && (key < 256)) /* Normal key */
			{
				type_key(b,key);
				interested = 1;
			}
			else if (key == 0x1CD) /* Insert */
			{
				if (insertmode != INSERT_CODE)
					insertmode = INSERT_CODE;
				else
					insertmode = 0;
				interested = 1;
				bs = progs;
				while (bs)
				{
					bs->flags |= PROG_INVALIDSTATUS;
					bs = bs->next;
				}
			}
			else if (key == 0x1DD) /* shift insert */
			{
				insertmode ^= INSERT_STRING;
				interested = 1;
				bs = progs;
				while (bs)
				{
					bs->flags |= PROG_INVALIDSTATUS;
					bs = bs->next;
				}
			}
			else if (key == 0x1ED) /* ctrl insert */
			{
				if (insertmode != INSERT_SUPER)
					insertmode = INSERT_SUPER;
				else
					insertmode = 0;
				interested = 1;
				bs = progs;
				while (bs)
				{
					bs->flags |= PROG_INVALIDSTATUS;
					bs = bs->next;
				}
			}
			else if (key == 0x1FD) /* shift-ctrl insert */
			{
				insertmode ^= INSERT_LOCK;
				interested = 1;
				bs = progs;
				while (bs)
				{
					bs->flags |= PROG_INVALIDSTATUS;
					bs = bs->next;
				}
			}
			if (interested)
			{
				interface_updatespace(b,b->cursor,0);
				b->flags |= PROG_INVALIDSTATUS;
				update_cursor(b);
				return 1;
			}
		}
		b = b->next;
	}
	if (!interested)
		LimpX_ProcessKey(key);
	return 1;
}

int func_break_click(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i)
{
	char *n;
	int tmp;
	if (i == LimpX_IconFromWindow(w,0)) /* Cancel */
	{
		LimpX_CreateMenu((limpx_menu *) NULL,0,0); /* Close menus */
		return 1;
	}
	else if (i == LimpX_IconFromWindow(w,1)) /* OK */
	{
		/* Read in the settings... */
		for (tmp=0;tmp<NUM_BREAKPOINTS;tmp++)
		{
			if (menu_space->breakflags & (1 << tmp))
				interface_updatespace(menu_space,menu_space->breaks[tmp],(int *) NULL);
			if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(w,2+tmp*2)) & LIMPX_ICONFLAG_SELECTED)
				menu_space->breakflags |= 1 << tmp;
			else
				menu_space->breakflags &= ~(1 << tmp);
			n = LimpX_GetIcon_Text(LimpX_IconFromWindow(w,3+tmp*2));
			parse_coord_string(menu_space,n,menu_space->breaks[tmp]);
			if (menu_space->breakflags & (1 << tmp))
				interface_updatespace(menu_space,menu_space->breaks[tmp],(int *) NULL);
		}
		LimpX_CreateMenu((limpx_menu *) NULL,0,0); /* Close menus */
		return 1;
	}
	return 0;
}

#endif
