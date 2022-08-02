#ifndef _INTER_FILE_C
#define _INTER_FILE_C

#include "file.h"
#include "funcs.h"
#include "main.h"
#include "update.h"

/*
   File handlers
*/

int func_save_ok(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i)
{
	/* Jump straight into data save */
	if (m->buttons != 4)
		return 0;
	func_do_save(LimpX_GetIcon_Text(LimpX_IconFromWindow(w,1)));
	return 1;
}

int func_drag_box_save(int minx,int miny,int maxx,int maxy)
{
	limpx_mouse_obj m;
	char *p,*op;
	int block[64];
#ifdef DEBUG
	fprintf(debuglog,"Drag end:\n");
#endif
	/* Remove current event */
	LimpX_DeleteEvent(event_drag_box);
#ifdef DEBUG
	fprintf(debuglog,"  Event removed\n");
#endif
	LimpX_DragASprite_Stop();
	/* Get mouse pos */
	LimpX_GetPointerInfo(&m);
	/* Need to get file leafname... */
	op = p = LimpX_GetIcon_Text(LimpX_IconFromWindow(win_xfer,1));
	while (p != NULL)
	{
		op = p;
		p = strchr(op,'.'); /* Find the next '.' */
		if (p != NULL)
			p++; /* Skip char */
	}
	block[0] = 45+strlen(op); /* 44 for message, string length, terminator */
	while (block[0] & 3)
		block[0]++;
	block[1] = 0;
	block[2] = 0;
	block[3] = 0; /* Your ref */
	block[4] = LIMPX_USERMSG_DATA_SAVE;
	block[5] = m.window;
	block[6] = m.icon;
	block[7] = m.x;
	block[8] = m.y;
	block[9] = 1; /* 'estimate' for the file size */
	block[10] = 0xffd;
	strcpy((char *) &block[11],op);
#ifdef DEBUG
	fprintf(debuglog,"  Message set up\n");
#endif
	LimpX_SendMessage(LIMPX_POLLCODE_USER_MESSAGE,block,m.window,m.icon);
#ifdef DEBUG
	fprintf(debuglog,"  Message sent\n");
#endif
	LimpX_CloseWindow(win_xfer);
	LimpX_CreateMenu((limpx_menu *) NULL,0,0); /* Close save menu */
#ifdef DEBUG
	fprintf(debuglog,"  Window closed, exiting\n");
#endif
	return 1;
}

int func_save_drag(limpx_mouse_obj *m,limpx_window *w,limpx_icon *i)
{
	int ox,oy;
	limpx_window_state p;
	int block[4];
	if (m->buttons != 64)
		return 0;
#ifdef DEBUG
	fprintf(debuglog,"Starting drag\n");
#endif
	p = LimpX_GetWindowState(w);
	ox = p.vminx - p.scrollx;
	oy = p.vmaxy - p.scrolly;
	LimpX_GetIconState(i,&temp_iconblock);
	LimpX_DragBox(w,5,temp_iconblock.sminx + ox,temp_iconblock.sminy + oy,temp_iconblock.smaxx + ox,temp_iconblock.smaxy + oy,0,0,-1,-1,0,0,0,0);
	block[0] = temp_iconblock.sminx + ox;
	block[1] = temp_iconblock.sminy + oy;
	block[2] = temp_iconblock.smaxx + ox;
	block[3] = temp_iconblock.smaxy + oy;
	LimpX_DragASprite_Start(0x85,1,"file_fff",block,NULL);
	/* Add drag event */
	event_drag_box = LimpX_NewEvent_Drag_Box(func_drag_box_save);
#ifdef DEBUG
	fprintf(debuglog,"Func end\n");
#endif
	return 1;
}

int func_save_msg_ok(int code,int *block)
{
	/* Call save code */
#ifdef DEBUG
	fprintf(debuglog,"Save OK message!\n");
#endif
	func_do_save((char *) &block[11]);
	return 1;
}

int func_load_handler(int code,int *block)
{
	limpx_icon *i;
	limpx_window *w;
	bspace *b;
	int s[MAX_DIM],tmp;
	w = LimpX_WindowFromHandle(block[5]);
	i = LimpX_IconFromHandle(block[5],block[6]);
#ifdef DEBUG
	fprintf(debuglog,"Load handler called! %X,%X\n",block[5],block[6]);
#endif
	if (w == win_newfile)
	{
		LimpX_SetIcon_Text(LimpX_IconFromWindow(win_newfile,1),(char *) &block[11]);
		LimpX_SetIcon_TextLength(LimpX_IconFromWindow(win_newfile,1),256);
		/* Respond to message */
		block[3] = block[2]; /* Fill in our ref */
		block[4] = LIMPX_USERMSG_DATA_LOAD_ACK;
		LimpX_SendMessage(code,block,block[1],0);
		return 1;
	}
	else if ((i == icon_bar) || (block[5] == -1))
	{
		/* Open new file window */
		func_fiddle_newfile_window((char *) &block[11],"");
		LimpX_OpenWindow(win_newfile);
		/* Respond to message */
		block[3] = block[2]; /* Fill in our ref */
		block[4] = LIMPX_USERMSG_DATA_LOAD_ACK;
		LimpX_SendMessage(code,block,block[1],0);
		return 1;
	}
	else
	{
		/* Search for fungespace window */
		b=progs;
		while (b)
		{
			if (b->win_main == w)
			{
				/* Drag to fungespace */
				/* Load as text file */
				/* Just load over cursor for now */
				if (load_file(b,(char *) &block[11],b->cursor,0,s) == EOF)
					report_error("Unable to load file");
				else
				{
					for (tmp=0;tmp<b->dims;tmp++)
						b->cursor[tmp] += s[tmp]; /* Move cursor */
					LimpX_ForceRedraw(w,0,-32000,32000,0);
					update_cursor(b); /* Ensure on-screen */
				}
				/* Respond */
				block[3] = block[2]; /* Fill in our ref */
				block[4] = LIMPX_USERMSG_DATA_LOAD_ACK;
				LimpX_SendMessage(code,block,block[1],0);
				return 1;
			}
			b = b->next;
		}
	}
	return 0;
}

#endif
