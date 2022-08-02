#ifndef _SEM_ORTH_C
#define _SEM_ORTH_C

#include "orth.h"

static int orth_a(bspace *b,bip *ip,int cell)
{
	int x,y;
	x = bef_pop(ip);
	y = bef_pop(ip);
	bef_push(ip,x & y);
	return EXEC_NEXTINSTR;
}

static int orth_e(bspace *b,bip *ip,int cell)
{
	int x,y;
	x = bef_pop(ip);
	y = bef_pop(ip);
	bef_push(ip,x ^ y);
	return EXEC_NEXTINSTR;
}

static int orth_g(bspace *b,bip *ip,int cell)
{
	int tpos[MAX_DIM],tmp;
	for (tmp=0;tmp<b->dims;tmp++)
		tpos[tmp] = bef_pop(ip);
	bef_push(ip,bef_readspace(b,tpos,ip->origin));
	return EXEC_NEXTINSTR;
}

static int orth_o(bspace *b,bip *ip,int cell)
{
	int x,y;
	x = bef_pop(ip);
	y = bef_pop(ip);
	bef_push(ip,x | y);
	return EXEC_NEXTINSTR;
}

static int orth_p(bspace *b,bip *ip,int cell)
{
	int tpos[MAX_DIM],tmp;
	for (tmp=0;tmp<b->dims;tmp++)
		tpos[tmp] = bef_pop(ip);
	bef_setspace(b,tpos,ip->origin,bef_pop(ip));
	return EXEC_NEXTINSTR;
}

static int orth_s(bspace *b,bip *ip,int cell)
{
	/* Output chars until 0 found */
	cell = bef_pop(ip);
	while (cell)
	{
		if (bef_putchar(b,cell) == EOF)
		{
			bef_reflectip(b,ip); /* Error! */
			return EXEC_NEXTINSTR;
		}
		cell = bef_pop(ip);
	}
	return EXEC_NEXTINSTR;
}

static int orth_v(bspace *b,bip *ip,int cell)
{
	ip->delta[0] = bef_pop(ip);
	bef_flycheck(b,ip);
	return EXEC_NEXTINSTR;
}

static int orth_w(bspace *b,bip *ip,int cell)
{
	if (b->dims < 2)
		bef_unimplemented(b,ip,cell);
	else
	{
		ip->delta[1] = bef_pop(ip);
		bef_flycheck(b,ip);
	}
	return EXEC_NEXTINSTR;
}

static int orth_x(bspace *b,bip *ip,int cell)
{
	ip->pos[0] = bef_pop(ip);
	if (b->sizes[0])
	{
		ip->pos[0] = ip->pos[0] % b->sizes[0];
		if (ip->pos[0] < 0)
			ip->pos[0] += b->sizes[0];
	}
	return EXEC_NEXTINSTR;
}

static int orth_y(bspace *b,bip *ip,int cell)
{
	if (b->dims < 1)
		bef_unimplemented(b,ip,cell);
	else
	{
		ip->pos[1] = bef_pop(ip);
		if (b->sizes[1])
		{
			ip->pos[1] = ip->pos[1] % b->sizes[1];
			if (ip->pos[1] < 0)
				ip->pos[1] += b->sizes[1];
		}
	}
	return EXEC_NEXTINSTR;
}

static int orth_z(bspace *b,bip *ip,int cell)
{
	if (bef_pop(ip) == 0)
		bef_nextpos(b,ip,0);
	return EXEC_NEXTINSTR;
}

int install_orth(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,ORTH_FINGER,'A',orth_a,load);
	bef_overloadsem(ip,ORTH_FINGER,'E',orth_e,load);
	bef_overloadsem(ip,ORTH_FINGER,'G',orth_g,load);
	bef_overloadsem(ip,ORTH_FINGER,'O',orth_o,load);
	bef_overloadsem(ip,ORTH_FINGER,'P',orth_p,load);
	bef_overloadsem(ip,ORTH_FINGER,'S',orth_s,load);
	bef_overloadsem(ip,ORTH_FINGER,'V',orth_v,load);
	bef_overloadsem(ip,ORTH_FINGER,'W',orth_w,load);
	bef_overloadsem(ip,ORTH_FINGER,'X',orth_x,load);
	bef_overloadsem(ip,ORTH_FINGER,'Y',orth_y,load);
	bef_overloadsem(ip,ORTH_FINGER,'Z',orth_z,load);
	return 1;
}

#endif
