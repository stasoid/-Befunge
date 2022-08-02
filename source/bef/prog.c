#ifndef _BEF_PROG_C
#define _BEF_PROG_C

#include "prog.h"
#include "mem.h"
#include "ip.h"
#include "sem.h"
#include "io.h"

bspace *progs;

/* Befunge space/progs */

bspace *bef_newprog(int dims,int *sizes,int bits)
{
	int c;
	bspace *n;
	n = malloc(sizeof(bspace));
	n->next = progs;
	n->prev = (bspace *) NULL;
	progs = n;
	if (n->next != NULL)
		n->next->prev = n;
	n->dims = dims;
	c = 1;
	for (dims=0;dims<n->dims;dims++)
	{
		n->sizes[dims] = sizes[dims];
		c *= n->sizes[dims];
	}
	for (dims=n->dims;dims<MAX_DIM;dims++)
		n->sizes[dims] = 1; /* Zero other values */
	n->bits = bits;
	bef_initcells(n);
	n->flags = 0;
	n->ips = (bip *) NULL;
#ifndef LYT
	n->win_main = n->win_tools = n->win_io = n->win_ip = (limpx_window *) NULL;
#endif
	for (c=0;c<MAX_DIM;c++)
		n->cursor[c] = 0;
	n->cdir = 0;
	for (c=0;c<80*25;c++)
		n->text_buf[c] = 32; /* Fill with spaces */
	n->text_x = n->text_y = 0;
	n->text_input[0] = 0;
	n->textbuf_chars = 0;
	n->input_ip = (bip *) NULL;
	n->breakflags = 0;
	for (c=0;c<NUM_BREAKPOINTS;c++)
		for (dims=0;dims<n->dims;dims++)
			n->breaks[c][dims] = 0;
	n->spool = (FILE *) NULL;
	n->haunted = (bspace *) NULL;
	n->args[0] = 0;
	return n;
}

void bef_killprog(bspace *b)
{
	while (b->ips != NULL)
		bef_killip(b,b->ips);
	bef_killprogsem(b);
	bef_killcells(b);
	if (b->next != NULL)
		b->next->prev = b->prev;
	if (b->prev != NULL)
		b->prev->next = b->next;
	else
		progs = b->next;
	if (b->spool)
		fclose(b->spool);
#ifndef LYT
	if (b->win_main)
		LimpX_DeleteWindow(b->win_main);
	if (b->win_tools)
		LimpX_DeleteWindow(b->win_tools);
	if (b->win_io)
		LimpX_DeleteWindow(b->win_io);
	if (b->win_ip)
		LimpX_DeleteWindow(b->win_ip);
#endif
	free(b);
}

void bef_resetprog(bspace *b)
{
	/* Kill all IPs, clear IO, etc. */
	while (b->ips != NULL)
		bef_killip(b,b->ips);
	bef_killprogsem(b);
	bef_killcells(b);
	bef_initcells(b);
	b->text_input[0] = 0;
	b->textbuf_chars = 0;
	bef_cls(b);
	if (b->spool)
		fprintf(b->spool,"\n*** Program reset\n");
}

#endif
