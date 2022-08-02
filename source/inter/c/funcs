#ifndef _INTER_FUNCS_C
#define _INTER_FUNCS_C

#include "funcs.h"
#include "main.h"

/*
   Reused functions
*/

void func_do_save(char *name) /* save menu_space with supplied file name */
{
	limpx_window_state s;
	FILE *f;
	int x,y,ox;
	if (savetype == SAVETYPE_NORM)
	{
		if (save_file(menu_space,name,0,0,1) == EOF)
		{
			report_error("Unable to save file");
			return;
		}
		strcpy(menu_space->fname,name);
		LimpX_SetWindow_Title(menu_space->win_main,menu_space->fname);
		LimpX_SetWindow_Title(menu_space->win_io,menu_space->fname);
		LimpX_SetWindow_Title(menu_space->win_ip,menu_space->fname);
		s = LimpX_GetWindowOutline(menu_space->win_main);
		LimpX_ForceRedraw((limpx_window *) NULL,s.vminx,s.vmaxy-64,s.vmaxx,s.vmaxy);
		s = LimpX_GetWindowOutline(menu_space->win_io);
		LimpX_ForceRedraw((limpx_window *) NULL,s.vminx,s.vmaxy-64,s.vmaxx,s.vmaxy);
		s = LimpX_GetWindowOutline(menu_space->win_ip);
		LimpX_ForceRedraw((limpx_window *) NULL,s.vminx,s.vmaxy-64,s.vmaxx,s.vmaxy);
		LimpX_CreateMenu((limpx_menu *) NULL,0,0);
		return;
	}
	f = fopen(name,"w");
	if (f == NULL)
	{
		report_error("Unable to save file");
		return;
	}
	for (y=0;y<=menu_space->text_y;y++)
	{
		ox = 0;
		for (x=0;x<80;x++)
			if (menu_space->text_buf[x+(80*y)] != ' ')
			{
				while (ox++ < x)
					fputc(' ',f);
				fputc(menu_space->text_buf[x+(80*y)],f);
			}
		if ((y != menu_space->text_y) || (ox == 79))
			fputc('\n',f);
	}
	if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_xfer,3)) & LIMPX_ICONFLAG_SELECTED)
		menu_space->spool = f;
	else
		fclose(f);
}

void func_fiddle_newfile_window(char *fname,char *args)
{
	/* If fname or args are null, leave as is */
	limpx_icon *i;
	if (fname)
	{
		LimpX_SetIcon_Text(LimpX_IconFromWindow(win_newfile,1),fname);
		LimpX_SetIcon_TextLength(LimpX_IconFromWindow(win_newfile,1),256);
	}
	if (args)
	{
		LimpX_SetIcon_Text(LimpX_IconFromWindow(win_newfile,12),args);
		LimpX_SetIcon_TextLength(LimpX_IconFromWindow(win_newfile,12),256);
	}
	/* Ensure dimensions can be set */
	LimpX_SetIcon_TextLength(LimpX_IconFromWindow(win_newfile,3),36);
	if (LimpX_GetIcon_Flags(LimpX_IconFromWindow(win_newfile,4)) & LIMPX_ICONFLAG_SELECTED)
	{ /* 98 is on */
		/* Hide 14,15,16,17, show 5,8,11,12,20,21 */
		i = LimpX_IconFromWindow(win_newfile,14);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,15);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,16);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,17);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,5);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,8);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,11);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,12);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,20);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,21);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
	}
	else
	{ /* 98 is off */
		i = LimpX_IconFromWindow(win_newfile,14);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,15);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,16);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,17);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) & ~LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,5);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,8);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,11);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,12);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,20);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
		i = LimpX_IconFromWindow(win_newfile,21);
		LimpX_SetIcon_Flags(i,LimpX_GetIcon_Flags(i) | LIMPX_ICONFLAG_DELETED);
	}
}

void open_funge_menu(bspace *b,limpx_mouse_obj *m) /* Open funge menu, using supplied mouse ptr or 0,0 if none */
{
	int t,t2;
	if (b->flags & PROG_98)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_98,1 | LIMPX_MENUITEM_DOTTED);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_98,LIMPX_MENUITEM_DOTTED);
	if (b->flags & PROG_FS)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_files,1);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_files,0);
	if (b->flags & PROG_CON)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_concurrent,1);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_concurrent,0);
	if (b->flags & PROG_SCRIPT)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_script,1);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_script,0);
	if (b->flags & PROG_NOPROMPT)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_noprompt,1);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_noprompt,0);
	if (b->flags & PROG_WARN_UNIMPLEMENTED)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_warn,1);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_warn,0);
	if (b->flags & PROG_UP_IS_UP)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_up,1);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_up,0);
	if (b->flags & PROG_INPUT_PAUSES)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_inpause,1);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_inpause,0);
	if (b->bits == 32)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_32,1);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_32,0);
	t = -1;
	for (t2=0;t2<NUM_BREAKPOINTS;t2++)
		if (b->breakflags & (1 << t2))
			if (memcmp(b->breaks[t2],menu_pos,b->dims*4) == 0)
				t = t2;
	if (t != -1)
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_breakhere,1);
	else
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_breakhere,0);
	if (b->spool)
	{
		LimpX_SetMenuItem_ClearSub(menu_funge,menu_funge_spool);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_spool,1+LIMPX_MENUITEM_DOTTED);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_spool,"Spool output");
	}
	else
	{
		LimpX_SetMenuItem_SubWindow(menu_funge,menu_funge_spool,win_xfer);
		LimpX_SetMenuItem_ItemFlags(menu_funge,menu_funge_spool,LIMPX_MENUITEM_MESSAGE+LIMPX_MENUITEM_DOTTED);
		LimpX_SetMenuItem_Text(menu_funge,menu_funge_spool,"Save output");
	}
	LimpX_UpdateMenuBlock(menu_funge);
	if (m)
		LimpX_CreateMenu(menu_funge,m->x-64,m->y+(44*3));
	else
		LimpX_CreateMenu(menu_funge,0,0);
	open_menu = MENU_FUNGE;
	menu_space = b;
}

void deselect_tool(bspace *b,int i)
{
	limpx_icon *ic;
	ic = LimpX_IconFromWindow(b->win_tools,i);
	LimpX_SetIcon_Flags(ic,LimpX_GetIcon_Flags(ic) & ~LIMPX_ICONFLAG_SELECTED);
}

void select_tool(bspace *b,int i)
{
	limpx_icon *ic;
	ic = LimpX_IconFromWindow(b->win_tools,i);
	LimpX_SetIcon_Flags(ic,LimpX_GetIcon_Flags(ic) | LIMPX_ICONFLAG_SELECTED);
}

void interface_goto(bspace *b,int *pos)
{
	/* Ensure pos is on screen */
	limpx_window_state s;
	int tmp,up;
	int x,y;
	s = LimpX_GetWindowState(b->win_main);
	up = 0;
	for (tmp=2;tmp<b->dims;tmp++)
		if (b->cursor[tmp] != pos[tmp])
		{
			up = 1;
			b->cursor[tmp] = pos[tmp]; /* Ensure cursor is kept on level */
		}
	x = (pos[0]*XSCALE)+XSCALE/2; /* Use mid point */
	y = -((pos[1]*YSCALE)+YSCALE/2);
	if ((x < s.scrollx) || (x > s.scrollx+(s.vmaxx-s.vminx)) || (y > s.scrolly-56) || (y < s.scrolly-(s.vmaxy-s.vminy)))
		up = 1;
	if (up)
	{
		s.scrollx = pos[0]*XSCALE-((s.vmaxx-s.vminx)/2);
		s.scrolly = pos[1]*-YSCALE+((s.vmaxy-s.vminy)/2);
		LimpX_OpenWindowAt(b->win_main,s);
	}
}

void parse_coord_string(bspace *b,char *n,int *pos)
{
	int tmp,tmp2;
	/* Parse some coordinates */
	for (tmp=0;tmp<MAX_DIM;tmp++)
		pos[tmp] = 0;
	tmp = 0; /* Current dim */
	tmp2 = 1; /* sign */
	while ((*n) && (tmp < b->dims))
	{
		if (*n == '-')
			tmp2 = -1;
		else if ((*n >= '0') && (*n <= '9'))
			pos[tmp] = pos[tmp]*10 + (*n - '0')*tmp2;
		else /* Anything else is seperator */
		{
			tmp++;
			tmp2 = 1;
		}
		n++;
	}
}

#endif
