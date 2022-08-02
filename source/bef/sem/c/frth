#ifndef _SEM_FRTH_CC
#define _SEM_FRTH_CC

#include "frth.h"

static int frth_d(bspace *b,bip *ip,int cell)
{
	bef_push(ip,bef_stacksize(ip,0));
	return EXEC_NEXTINSTR;
}

static int frth_l(bspace *b,bip *ip,int cell)
{
	int n,t;
	int *z;
	n = bef_pop(ip);
	if (n >= bef_stacksize(ip,0))
		bef_push(ip,0);
	else if (n < 0)
		bef_reflectip(b,ip);
	else if (n > 0)
	{
		/* We need the roll part to work according to queuemode */
		/* So if queuemode is on, temporarily turn invertmode on */
		cell = ip->flags & IP_INVERTMODE;
		if (ip->flags & IP_QUEUEMODE)
			ip->flags |= IP_INVERTMODE;
		else
			ip->flags &= ~IP_INVERTMODE;
		z = malloc(sizeof(int)*n);
		for (t=0;t<n;t++)
			z[t] = bef_pop(ip);
		for (t=n-2;t>=0;t--)
			bef_push(ip,z[t]);
		/* Restore invertmode */
		ip->flags = (ip->flags & ~IP_INVERTMODE) | cell;
		bef_push(ip,z[n-1]);
		free(z);
	}
	return EXEC_NEXTINSTR;
}

static int frth_o(bspace *b,bip *ip,int cell)
{
	int a,bb;
	bb = bef_pop(ip);
	a = bef_pop(ip);
	bef_push(ip,a);
	bef_push(ip,bb);
	bef_push(ip,a);
	return EXEC_NEXTINSTR;
}

static int frth_p(bspace *b,bip *ip,int cell)
{
	/* Pick stack */
	cell = bef_pop(ip);
	if (cell < 0)
		bef_reflectip(b,ip);
	else
		bef_push(ip,bef_peekstack(ip,cell));
	return EXEC_NEXTINSTR;
}

static int frth_r(bspace *b,bip *ip,int cell)
{
	int a,bb,c;
	c = bef_pop(ip);
	bb = bef_pop(ip);
	a = bef_pop(ip);
	bef_push(ip,bb);
	bef_push(ip,c);
	bef_push(ip,a);
	return EXEC_NEXTINSTR;
}

int install_frth(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,FRTH_FINGER,'D',frth_d,load);
	bef_overloadsem(ip,FRTH_FINGER,'L',frth_l,load);
	bef_overloadsem(ip,FRTH_FINGER,'O',frth_o,load);
	bef_overloadsem(ip,FRTH_FINGER,'P',frth_p,load);
	bef_overloadsem(ip,FRTH_FINGER,'R',frth_r,load);
	return 1;
}

#endif
