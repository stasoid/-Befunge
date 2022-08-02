#ifndef _SEM_MODU_C
#define _SEM_MODU_C

#include <math.h>

#include "modu.h"

static int modu_m(bspace *b,bip *ip,int cell)
{
	int x,y; /* Signed result modulo */
	y = bef_pop(ip);
	x = bef_pop(ip);
	bef_push(ip,(int) (x - floor(x/y)*y));
	return EXEC_NEXTINSTR;
}

static int modu_u(bspace *b,bip *ip,int cell)
{
	int x,y; /* Sam Holden's unsigned-result modulo */
	/* Presumably just add y if <0 */
	y = bef_pop(ip);
	x = bef_pop(ip);
	if ((x % y) < 0)
		bef_push(ip,(x % y)+y);
	else
		bef_push(ip,x % y);
	return EXEC_NEXTINSTR;
}

static int modu_r(bspace *b,bip *ip,int cell)
{
	int x,y;
	y = bef_pop(ip);
	x = bef_pop(ip);
	/* C integer remainder */
	/* Just % then? */
	bef_push(ip,x % y);
	return EXEC_NEXTINSTR;
}

int install_modu(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,MODU_FINGER,'M',modu_m,load);
	bef_overloadsem(ip,MODU_FINGER,'U',modu_u,load);
	bef_overloadsem(ip,MODU_FINGER,'R',modu_r,load);
	return 1;
}

#endif
