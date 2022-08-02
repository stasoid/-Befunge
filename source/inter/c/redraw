#ifndef _INTER_REDRAW_C
#define _INTER_REDRAW_C

#include "redraw.h"
#include "../bef/mem.h"
#include "main.h"
#include "../bef/stack.h"
#include "../bef/prog.h"

/*
   Redraw code
*/

void redraw_block(bspace *b,limpx_redraw_obj *r,int x,int y)
{
	int blk;
	int pos[MAX_DIM];
	for (blk=2;blk<MAX_DIM;blk++)
		pos[blk] = b->cursor[blk]; /* We can never leave the plane the cursor is on */
	pos[0] = x;
	pos[1] = y;
	/* Unefunge fix */
	if ((b->dims > 1) || (pos[1] == 0))
		blk = (int) bef_readspace(b,pos,0);
	else
		blk = 32; /* Dims=1, pos[1] != 0 */
	if ((blk >= 0) && (blk < 32))
	{
		LimpX_SetColour(11);
		printf("%c",blk+'0');
		LimpX_SetColour(7);
	}
	else if (blk == 127)
	{
		LimpX_SetColour(11);
		printf("ˆ");
		LimpX_SetColour(7);
	}
	else if ((blk > 255) || (blk < 0))
	{
		LimpX_SetColour(11);
		printf("ÿ");
		LimpX_SetColour(7);
	}
	else
		printf("%c",blk); /* Must be printable by now */
	if ((x == b->cursor[0]) && (y == b->cursor[1]))
	{
		/* Draw cursor */
		LimpX_SetColour(11+0x30);
		regs.r[0] = 4+96;
		regs.r[1] = (r->vminx-r->scrollx)+(x*XSCALE);
		regs.r[2] = (r->vmaxy-r->scrolly)-(y*YSCALE);
		_kernel_swi(OS_Plot,&regs,&regs);
		regs.r[0] = 1+96;
		regs.r[1] = XSCALE;
		regs.r[2] = -YSCALE;
		_kernel_swi(OS_Plot,&regs,&regs);
		LimpX_SetColour(7);
		b->flags = b->flags | PROG_INVALIDSTATUS; /* Update status line in the future */
		regs.r[0] = 4;
		regs.r[1] = (r->vminx-r->scrollx)+((x+1)*XSCALE);
		regs.r[2] = (r->vmaxy-r->scrolly)-(y*YSCALE);
		_kernel_swi(OS_Plot,&regs,&regs);
	}
}

void interface_redraw(bspace *b,limpx_redraw_obj *r)
{
	int minx,miny,maxx,maxy,x,y;
	bip *ip;
	int tmp;
	minx = r->rminx + (r->scrollx - r->vminx);
	miny = r->rminy + (r->scrolly - r->vmaxy);
	maxx = r->rmaxx + (r->scrollx - r->vminx);
	maxy = r->rmaxy + (r->scrolly - r->vmaxy);
	/* Now invert y... */
	x = miny;
	miny = -maxy;
	maxy = -x;
	/* Convert to coordinates instead of pixels */
	minx = (minx / XSCALE)-1;
	miny = (miny / YSCALE)-1;
	maxx = (maxx / XSCALE)+1;
	maxy = (maxy / YSCALE)+1;
	/* Now redraw */
	LimpX_SetColour(7);
	for (y=miny;y<maxy;y++)
	{
		regs.r[0] = 4;
		regs.r[1] = (r->vminx-r->scrollx)+(minx*XSCALE);
		regs.r[2] = (r->vmaxy-r->scrolly)-(y*YSCALE);
		_kernel_swi(OS_Plot,&regs,&regs);
		for (x=minx;x<maxx;x++)
			redraw_block(b,r,x,y);
	}
	/* Redraw IPs and breakpoints */
	/* Quicker to do it here than for each block */
	ip = b->ips;
	while (ip != NULL)
	{
		if ((b->dims < 3) || (memcmp(&ip->pos[2],&b->cursor[2],4*(MAX_DIM-2)) == 0))
		{
			/* Draw IP */
			LimpX_SetColour(8+0x30);
			regs.r[0] = 4+96;
			regs.r[1] = (r->vminx-r->scrollx)+(ip->pos[0]*XSCALE);
			regs.r[2] = (r->vmaxy-r->scrolly)-(ip->pos[1]*YSCALE);
			_kernel_swi(OS_Plot,&regs,&regs);
			regs.r[0] = 1+96;
			regs.r[1] = XSCALE;
			regs.r[2] = -YSCALE;
			_kernel_swi(OS_Plot,&regs,&regs);
			LimpX_SetColour(7);
		}
		ip = ip->next;
	}
	for (tmp=0;tmp<NUM_BREAKPOINTS;tmp++)
	{
		if (b->breakflags & (1 << tmp))
			if ((b->dims < 3) || (memcmp(&b->breaks[tmp][2],&b->cursor[2],4*(MAX_DIM-2)) == 0))
			{
				/* Draw breakpoint */
				/* Do it as a box around the cell */
				x = (r->vminx-r->scrollx)+(b->breaks[tmp][0]*XSCALE);
				y = (r->vmaxy-r->scrolly)-(b->breaks[tmp][1]*YSCALE);
				regs.r[0] = 4;
				regs.r[1] = x;
				regs.r[2] = y;
				_kernel_swi(OS_Plot,&regs,&regs);
				regs.r[0] = 1;
				regs.r[1] = XSCALE-1;
				regs.r[2] = 0;
				_kernel_swi(OS_Plot,&regs,&regs);
				regs.r[0] = 1;
				regs.r[1] = 0;
				regs.r[2] = 1-YSCALE;
				_kernel_swi(OS_Plot,&regs,&regs);
				regs.r[0] = 1;
				regs.r[1] = 1-XSCALE;
				regs.r[2] = 0;
				_kernel_swi(OS_Plot,&regs,&regs);
				regs.r[0] = 1;
				regs.r[1] = 0;
				regs.r[2] = YSCALE-1;
				_kernel_swi(OS_Plot,&regs,&regs);
			}
	}
}

void io_redraw(bspace *b,limpx_redraw_obj *r)
{
	int minx,miny,maxx,maxy,x,y;
	minx = r->rminx + (r->scrollx - r->vminx);
	miny = r->rminy + (r->scrolly - r->vmaxy);
	maxx = r->rmaxx + (r->scrollx - r->vminx);
	maxy = r->rmaxy + (r->scrolly - r->vmaxy);
	/* Now invert y... */
	x = miny;
	miny = -maxy;
	maxy = -x;
	/* Convert to coordinates instead of pixels */
	minx = (minx / XSCALE)-1;
	miny = (miny / YSCALE)-1;
	maxx = (maxx / XSCALE)+1;
	maxy = (maxy / YSCALE)+1;
	/* Now redraw */
	LimpX_SetColour(7);
	if (minx < 0)
		minx = 0;
	if (maxx > 80)
		maxx = 80;
	if (miny < 0)
		miny = 0;
	if (maxy > 25)
		maxy = 25;
	for (y=miny;y<maxy;y++)
	{
		regs.r[0] = 4;
		regs.r[1] = (r->vminx-r->scrollx)+(minx*XSCALE);
		regs.r[2] = (r->vmaxy-r->scrolly)-(y*YSCALE);
		_kernel_swi(OS_Plot,&regs,&regs);
		for (x=minx;x<maxx;x++)
		{
			printf("%c",b->text_buf[x+80*y]);
			if ((x == b->text_x) && (y == b->text_y))
			{
				if (b->input_ip != NULL)
					LimpX_SetColour(11+0x30);
				else
					LimpX_SetColour(8+0x30);
				regs.r[0] = 4+96;
				regs.r[1] = (r->vminx-r->scrollx)+(x*XSCALE);
				regs.r[2] = (r->vmaxy-r->scrolly)-(y*YSCALE);
				_kernel_swi(OS_Plot,&regs,&regs);
				regs.r[0] = 5+96;
				regs.r[1] = (r->vminx-r->scrollx)+((x+1)*XSCALE);
				regs.r[2] = (r->vmaxy-r->scrolly)-((y+1)*YSCALE);
				_kernel_swi(OS_Plot,&regs,&regs);
				LimpX_SetColour(7);
				regs.r[0] = 4;
				regs.r[1] = (r->vminx-r->scrollx)+((x+1)*XSCALE);
				regs.r[2] = (r->vmaxy-r->scrolly)-(y*YSCALE);
				_kernel_swi(OS_Plot,&regs,&regs);
			}
		}
	}
}

void ip_redraw(bspace *b,limpx_redraw_obj *r)
{
	int miny,maxy,tmp,tmp2,tmp3,h;
	bip *ip;
	char *delplus=">vl",*delminus="<^h";
	char buf[256]; /* For writing text into */
	char *cpos;
	limpx_icon_block icons[6]; /* The heading icons */
	if (UP_DIR(b) == -1)
	{
		delplus[2] = 'l';
		delminus[2] = 'h';
	}
	else
	{
		delplus[2] = 'h';
		delminus[2] = 'l';
	}
	h = 0;
	for (tmp=0;tmp<6;tmp++)
	{
		LimpX_GetIconState(LimpX_IconFromWindow(b->win_ip,tmp),&icons[tmp]);
		icons[tmp].flags |= 256; /* Indirected text */
		icons[tmp].data[0] = (int) buf;
		icons[tmp].data[1] = 0; /* No validation string */
		icons[tmp].data[2] = 256; /* Buffer length */
		if (tmp == 0)
			h = icons[0].sminy-icons[0].smaxy;
		icons[tmp].sminy += h; /* Move down to location of 1st ip */
		icons[tmp].smaxy += h;
	}
	miny = r->rminy + (r->scrolly - r->vmaxy); /* Area we are redrawing */
	maxy = r->rmaxy + (r->scrolly - r->vmaxy);
	ip = b->ips;
	while ((icons[0].smaxy > miny) && (ip))
	{
		if (icons[0].sminy < maxy)
		{
			/* Draw! */
			/* ID number */
			sprintf(buf,"%d",ip->id);
			LimpX_PlotIcon(&icons[0]);
			/* Location */
			cpos = buf;
			cpos += sprintf(buf,"(%d",ip->pos[0]);
			for (tmp=1;tmp<b->dims;tmp++)
				cpos += sprintf(cpos,",%d",ip->pos[tmp]);
			sprintf(cpos,")");
			LimpX_PlotIcon(&icons[1]);
			/* Flags */
			cpos = buf;
			buf[0] = 0;
			for (tmp=0;tmp<b->dims;tmp++)
				if (ip->delta[tmp] > 0)
					cpos += sprintf(cpos,"%c",delplus[tmp]);
				else if (ip->delta[tmp] < 0)
					cpos += sprintf(cpos,"%c",delminus[tmp]);
			if (ip->flags & IP_STRINGMODE)
				cpos += sprintf(cpos,"\"");
			else if (b->input_ip == ip)
				cpos += sprintf(cpos,"i");
			if (ip->flags & IP_HOVERMODE)
				cpos += sprintf(cpos,"H");
			if (ip->flags & IP_INVERTMODE)
				cpos += sprintf(cpos,"I");
			if (ip->flags & IP_QUEUEMODE)
				cpos += sprintf(cpos,"Q");
			if (ip->flags & IP_SWITCHMODE)
				cpos += sprintf(cpos,"S");
			if (b->flags & PROG_FINGERPRINT)
				cpos += sprintf(cpos,"F");
			if ((ip->ghost) && (ip->ghost->id > ip->id))
				cpos += sprintf(cpos,"G");
			if (ip->flags & IP_DEAD)
				cpos += sprintf(cpos,"D");
			LimpX_PlotIcon(&icons[2]);
			/* Offset */
			cpos = buf;
			cpos += sprintf(buf,"(%d",ip->origin[0]);
			for (tmp=1;tmp<b->dims;tmp++)
				cpos += sprintf(cpos,",%d",ip->origin[tmp]);
			sprintf(cpos,")");
			LimpX_PlotIcon(&icons[3]);
			/* Over */
			tmp=bef_readspace(b,ip->pos,(int *) NULL);
			if ((tmp > 31) && (tmp < 256) && (tmp != 127))
			{
				buf[0] = tmp;
				buf[1] = 0;
			}
			else
				sprintf(buf,"0x%x",tmp);
			LimpX_PlotIcon(&icons[4]);
			/* Top stack items */
			tmp3 = bef_stacksize(ip,0);
			cpos = buf;
			buf[0] = 0;
			for (tmp=0;(tmp<tmp3) && (tmp<10);tmp++)
			{
				tmp2 = bef_peekstack(ip,tmp);
				cpos += sprintf(cpos,"%d ",tmp2);
				if ((tmp2 > 31) && (tmp2 < 256) && (tmp2 != 127))
					cpos += sprintf(cpos,"('%c') ",tmp2);
			}
			if (tmp3 > 10)
				sprintf(cpos,"...");
			LimpX_PlotIcon(&icons[5]);
		}
		ip = ip->next;
		for (tmp=0;tmp<6;tmp++)
		{
			icons[tmp].sminy += h;
			icons[tmp].smaxy += h;
		}
	}
}

int func_gen_redraw(limpx_window *win)
{
	/* Look for the right befunge space... */
	limpx_redraw_obj *r;
	bspace *b;
	int tmp,h;
	b = progs;
	h = LimpX_HandleFromWindow(win);
	while ((b->win_main != win) && (b->win_io != win) && (b->win_ip != win))
	{
		if (b == NULL)
		{
			/* Hmm, that's strange. */
			r = malloc(sizeof(limpx_redraw_obj));
			r->ro_handle = h;
			tmp = LimpX_RedrawWindow(r);
			while (tmp != 0)
				tmp = LimpX_GetRectangle(r);
			free(r);
			return 1;
		}
		b = b->next;
	}
	/* Redraw... */
	if (b->win_io == win)
	{
		r = malloc(sizeof(limpx_redraw_obj));
		r->ro_handle = h;
		tmp = LimpX_RedrawWindow(r);
		while (tmp != 0)
		{
			io_redraw(b,r);
			tmp = LimpX_GetRectangle(r);
		}
		free(r);
	}
	else if (b->win_ip == win)
	{
		r = malloc(sizeof(limpx_redraw_obj));
		r->ro_handle = h;
		tmp = LimpX_RedrawWindow(r);
		while (tmp != 0)
		{
			ip_redraw(b,r);
			tmp = LimpX_GetRectangle(r);
		}
		free(r);
	}
	else
	{
		r = malloc(sizeof(limpx_redraw_obj));
		r->ro_handle = h;
		tmp = LimpX_RedrawWindow(r);
		while (tmp != 0)
		{
			interface_redraw(b,r);
			tmp = LimpX_GetRectangle(r);
		}
		free(r);
	}
	return 1;
}

int func_gen_openwindow(limpx_window *win,limpx_window_state *pos)
{
	bspace *b;
	limpx_window_state s;
	int icon_x,icon_min;
	limpx_icon *i;
	b = progs;
	while (b != NULL)
	{
		if (b->win_main == win)
		{
			/* Open tools */
			/* If main is behind tools, open as is */
			/* else if main is behind something else, move tools to */
			/* behind it and main to behind tools */
			s = LimpX_GetWindowState(b->win_tools);
			if (pos->behind != LimpX_HandleFromWindow(b->win_tools))
				s.behind = pos->behind; /* Move tools behind what the main window is behind */
			s.vminx = pos->vminx; /* Ensure tools are kept in top left */
			s.vminy = pos->vminy;
			s.vmaxx = pos->vmaxx;
			s.vmaxy = pos->vmaxy;
			LimpX_OpenWindowAt(b->win_tools,s);
			if (b->flags & PROG_FINGERPRINT)
				i = LimpX_IconFromWindow(b->win_tools,1);
			else
				i = LimpX_IconFromWindow(b->win_tools,6);
			icon_x = LimpX_GetIcon_MaxX(i);
			icon_min = LimpX_GetIcon_MinX(i);
			if (((pos->vmaxx-pos->vminx) > icon_min) && ((pos->vmaxx-pos->vminx) != icon_x)) /* Only change size if not current & not negative size */
			{
				LimpX_SetIcon_MaxX(i,pos->vmaxx-pos->vminx);
				if (b->flags & PROG_TOOLBARINVALID)
					LimpX_ForceRedraw(b->win_tools,icon_min,-128,pos->vmaxx-pos->vminx,0); /* Redraw whole icon if possibly dodgy */
				else
					LimpX_ForceRedraw(b->win_tools,icon_x-4,-128,pos->vmaxx-pos->vminx,0); /* ... else just the changed bit */
			}
			pos->behind = LimpX_HandleFromWindow(b->win_tools); /* Move main window behind tools window */
			LimpX_OpenWindowAt(b->win_main,*pos);
			b->flags |= PROG_TOOLBARINVALID; /* Check again next wimp_poll - this is a nasty hack to fix the adjust size icon bug */
			return 1;
		}
		b = b->next;
	}
	LimpX_OpenWindowAt(win,*pos);
	return 1;
}

int func_close_window(limpx_window *win)
{
	bspace *b,*bs,*nbs;
	b = progs;
	while (b != NULL)
	{
		if (b->win_main == win)
		{
			LimpX_CloseWindow(b->win_main);
			LimpX_CloseWindow(b->win_tools);
			if ((b->flags & PROG_FINGERPRINT) == 0)
				LimpX_CloseWindow(b->win_io);
			LimpX_CloseWindow(b->win_ip);
			if ((b->flags & PROG_FINGERPRINT) == 0)
				bef_killprog(b);
			bs = progs;
			while (bs)
			{
				nbs = bs->next;
				if ((bs->flags & PROG_FINGERPRINT) && (bs->haunted == b))
				{
					if (bs->win_main)
						func_close_window(bs->win_main);
					else
						bef_killprog(bs);
				}
				bs = nbs;
			}
			return 1;
		}
		b = b->next;
	}
	LimpX_CloseWindow(win);
	return 1;
}

#endif
