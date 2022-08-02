#ifndef _INTER_UPDATE_C
#define _INTER_UPDATE_C

#include "main.h"
#include "update.h"
#include "../bef/mem.h"
#include "funcs.h"

#define MIN(a,b) (a<b?a:b)

/*
   Update triggers
*/

void interface_updatespace(bspace *b,int *pos,int *by)
{
	int c;
	if ((nowimp) || (b->win_main == NULL))
		return;
	if (by == NULL)
	{
		if (b->dims > 2)
			for (c=2;c<b->dims;c++)
				if (pos[c] != b->cursor[c])
					return;
		if (b->dims == 1)
			LimpX_ForceRedraw(b->win_main,pos[0]*XSCALE,-YSCALE,((pos[0]+1)*XSCALE)+4,4);
		else
			LimpX_ForceRedraw(b->win_main,pos[0]*XSCALE,(-(pos[1]+1)*YSCALE),((pos[0]+1)*XSCALE)+4,(-pos[1]*YSCALE)+4);
		/* Check status bar */
		for (c=0;c<MIN(2,b->dims);c++)
			if (pos[c] != b->cursor[c])
				return;
		b->flags |= PROG_INVALIDSTATUS;
	}
	else
	{
		if (b->dims > 2)
			for (c=2;c<b->dims;c++)
				if (pos[c]+by[c] != b->cursor[c])
					return;
		if (b->dims == 1)
			LimpX_ForceRedraw(b->win_main,pos[0]*XSCALE,-YSCALE,((pos[0]+1)*XSCALE)+4,4);
		else
			LimpX_ForceRedraw(b->win_main,(pos[0]+by[0])*XSCALE,-(pos[1]+by[1]+1)*YSCALE,((pos[0]+by[0]+1)*XSCALE)+4,(-(pos[1]+by[1])*YSCALE)+4);
		for (c=0;c<MIN(2,b->dims);c++)
			if (pos[c]+by[c] != b->cursor[c])
				return;
		b->flags |= PROG_INVALIDSTATUS;
	}
}

void interface_updateblock(bspace *b,int *from,int *size)
{
	int c;
	if ((nowimp) || (b->win_main == NULL))
		return;
	if (b->dims > 2)
		for (c=2;c<MAX_DIM;c++)
			if ((from[c] > b->cursor[c]) || (from[c]+size[c] < b->cursor[c]))
				return;
	if (b->dims == 1)
		LimpX_ForceRedraw(b->win_main,from[0]*XSCALE,-YSCALE,((from[0]+size[0]+1)*XSCALE)+4,4);
	else
		LimpX_ForceRedraw(b->win_main,from[0]*XSCALE,-(from[1]+size[1]+1)*YSCALE,((from[0]+size[0]+1)*XSCALE)+4,(-from[1]*YSCALE)+4);
	/* Check status bar */
	for (c=0;c<MIN(2,b->dims);c++)
		if ((from[c] > b->cursor[c]) || (from[c]+size[c] < b->cursor[c]))
			return;
	b->flags |= PROG_INVALIDSTATUS;
}

void interface_refreshspace(bspace *b,int center)
{
	if ((nowimp) || (b->win_main == NULL))
		return;
	LimpX_ForceRedraw(b->win_main,-32000,-32000,32000,32000);
	LimpX_SetCaretPosition(b->win_main,(limpx_icon *) NULL,b->cursor[0]*XSCALE,b->cursor[1]*-YSCALE,-1,-1);
	if (center)
		interface_goto(b,b->cursor);
}

void interface_updateio(bspace *b,int x,int y,int center)
{
	limpx_window_state s;
	if ((nowimp) || (b->haunted))
		return;
	s = LimpX_GetWindowState(b->win_io);
	if ((s.flags & 0x10000) == 0)
		return; /* Not open */
	LimpX_ForceRedraw(b->win_io,(x*XSCALE)-4,(-(y+1)*YSCALE)-4,((x+1)*XSCALE)+4,(-y*YSCALE)+4);
	if (center == 0)
		return; /* Don't update cursor pos & reposition scroll offsets */
	LimpX_SetCaretPosition(b->win_io,(limpx_icon *) NULL,b->text_x*XSCALE,b->text_y*-YSCALE,-1,-1);
	if ((b->text_x*XSCALE < s.scrollx) || ((b->text_x+1)*XSCALE > s.scrollx+(s.vmaxx-s.vminx)) || ((b->text_y-2)*-YSCALE > s.scrolly) || (b->text_y*-YSCALE < s.scrolly-(s.vmaxy-s.vminy)))
	{
		/* Reposition scroll offsets... */
		/* Just stuff the cursor in the middle of the window for now */
		s.scrollx = b->text_x*XSCALE-((s.vmaxx-s.vminx)/2);
		s.scrolly = b->text_y*-YSCALE+((s.vmaxy-s.vminy)/2);
		LimpX_OpenWindowAt(b->win_io,s);
	}
}

void interface_refreshio(bspace *b)
{
	if ((nowimp) || (b->haunted))
		return;
	LimpX_ForceRedraw(b->win_io,0,-32000,32000,0);
}

void interface_statusalert(bspace *b,char *msg,int type)
{
	limpx_icon *i;
	limpx_icon_block ic;
	/* Display message on the status bar (or screen if singletasking) */
	/* Also switch to paused mode */
	/* Should only happen when the program is running, so don't need to worry about creating an initial IP or simultaneous messages */
	while (b->haunted)
		b = b->haunted;
	if (nowimp)
	{
		/* Display on screen */
		if (type & 1)
			printf("\n*** %s",msg);
		else
			return;
		if (type & 2)
		{
			printf(" (Press a key)\n");
			getc(stdin);
		}
		else
			printf("\n");
		return;
	}
	if (b->flags & PROG_FINGERPRINT)
		i = LimpX_IconFromWindow(b->win_tools,1);
	else
		i = LimpX_IconFromWindow(b->win_tools,6);
	LimpX_SetIcon_Text(i,msg);
	LimpX_SetIcon_Validation(i,"R6"); /* Borderd button */
	LimpX_GetIconState(i,&ic);
	LimpX_ForceRedraw(b->win_tools,ic.sminx,ic.sminy,ic.smaxx,ic.smaxy);
	if ((b->flags & PROG_FINGERPRINT) == 0)
	{
		b->flags |= PROG_STATUSALERT + PROG_PAUSED;
		b->flags &= ~PROG_RUNNING;
		deselect_tool(b,0); /* Play */
		select_tool(b,1); /* Pause */
		deselect_tool(b,2); /* Stop (shouldn't be pressed anyway) */
	}
}

void interface_update_status(bspace *b)
{
	/* Update the status line for this bspace */
	/* Need to display cursor pos & dec/hex/char value */
	char buff[64],*i;
	int c;
	limpx_icon_block ic;
	limpx_icon *ico;
	if (b->win_tools == NULL)
		return;
	if (b->flags & PROG_STATUSALERT)
		return; /* Keep alert visible */
	i = buff;
	i += sprintf(i,"(%d",b->cursor[0]);
	for (c=1;c<b->dims;c++)
		i += sprintf(i,", %d",b->cursor[c]);
	if (b->cdir == 0)
		i+=sprintf(i,") >");
	else if (b->cdir == 1)
		i+=sprintf(i,") v");
	else if (b->cdir == 2)
		i+=sprintf(i,") l");
	else if (b->cdir == 128)
		i+=sprintf(i,") <");
	else if (b->cdir == 129)
		i+=sprintf(i,") ^");
	else
		i+=sprintf(i,") h");
	if (b->input_ip != NULL)
		i+=sprintf(i,"i"); /* Input indicator */
	if (insertmode & INSERT_SUPER)
		i+=sprintf(i,"^");
	if (insertmode)
		i+=sprintf(i,"I");
	if (insertmode & INSERT_STRING)
		i+=sprintf(i,"\"");
	if (insertmode & INSERT_LOCK)
		i+=sprintf(i,"X");
	c = bef_readspace(b,b->cursor,(int *) NULL);
	i += sprintf(i," %d 0x%x",c,c);
	if ((c > 31) && (c < 256) && (c != 127))
		i += sprintf(i," '%c'",c);
	/* Now set text */
	if (b->flags & PROG_FINGERPRINT)
		ico = LimpX_IconFromWindow(b->win_tools,1);
	else
		ico = LimpX_IconFromWindow(b->win_tools,6);
	LimpX_SetIcon_Text(ico,buff);
	LimpX_GetIconState(ico,&ic);
	LimpX_ForceRedraw(b->win_tools,ic.sminx,ic.sminy,ic.smaxx,ic.smaxy); /* Only redraw the status line */
}

void func_update_ipwin(bspace *b)
{
	int ipcount;
	bip *ip;
	limpx_icon_block ib;
	limpx_window_header h;
	bspace *bs;
	if ((nowimp) || (b->win_ip == NULL))
		return;
	bs = progs;
	while (bs)
	{
		if ((bs->flags & PROG_FINGERPRINT) && (bs->haunted == b))
			func_update_ipwin(bs);
		bs = bs->next;
	}
	LimpX_GetWindowHeader(b->win_ip,&h);
	/* Update IP window, if open */
	if ((h.flags & LIMPX_WINFLAG_ISOPEN) == 0)
		return;
	ipcount = 1;
	ip = b->ips;
	while (ip)
	{
		ip = ip->next;
		ipcount++;
	}
	/* Now resize window */
	LimpX_GetIconState(LimpX_IconFromWindow(b->win_ip,0),&ib);
	/* Redraw */
	if ((ib.sminy+(ipcount*-(ib.smaxy-ib.sminy))+ib.smaxy) != h.wminy-h.wmaxy)
	{
		LimpX_SetExtent(b->win_ip,0,ib.sminy+(ipcount*-(ib.smaxy-ib.sminy))+ib.smaxy,80*XSCALE,0);
		LimpX_CloseWindow(b->win_ip); /* Nasty hack to redraw shrunken parts */
		LimpX_OpenWindowAt(b->win_ip,*((limpx_window_state *) &h));
	}
	else
	{
		LimpX_OpenWindowAt(b->win_ip,*((limpx_window_state *) &h));
		LimpX_ForceRedraw(b->win_ip,0,ib.sminy+(ipcount*-(ib.smaxy-ib.sminy)),80*XSCALE,ib.sminy);
	}
}

void update_cursor(bspace *b)
{
	LimpX_SetCaretPosition(b->win_main,(limpx_icon *) NULL,b->cursor[0]*XSCALE,b->cursor[1]*-YSCALE,-1,-1);
	interface_goto(b,b->cursor);
}

void interface_scroll(bspace *b)
{
	/* Scroll IO window */
	if (b->win_io == NULL)
		return;
	LimpX_BlockCopy(b->win_io,0,-25*YSCALE,80*XSCALE,-YSCALE,0,-24*YSCALE);
	LimpX_ForceRedraw(b->win_io,0,-25*YSCALE,80*XSCALE,-24*YSCALE);
}

#endif
