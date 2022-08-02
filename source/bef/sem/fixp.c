#ifndef _SEM_FIXP_C
#define _SEM_FIXP_C

#include <math.h>
#include "fixp.h"

#define RAD_PER_DEG 0.0174532925
#define PI 3.14159265

static int fixp_a(bspace *b,bip *ip,int cell)
{
	bef_push(ip,bef_pop(ip) & bef_pop(ip));
	return EXEC_NEXTINSTR;
}

static int fixp_b(bspace *b,bip *ip,int cell)
{
	double d;
	d = (double) bef_pop(ip);
	d = acos(d/10000)*572957;
	bef_push(ip,(int) d);
	return EXEC_NEXTINSTR;
}

static int fixp_c(bspace *b,bip *ip,int cell)
{
	double d;
	d = (double) bef_pop(ip);
	d /= 10000;
	d = cos(d*RAD_PER_DEG)*10000;
	bef_push(ip,(int) d);
	return EXEC_NEXTINSTR;
}

static int fixp_d(bspace *b,bip *ip,int cell)
{
	bef_push(ip,(int) (((double) rand())/bef_pop(ip)));
	return EXEC_NEXTINSTR;
}

static int fixp_i(bspace *b,bip *ip,int cell)
{
	double d;
	d = (double) bef_pop(ip);
	d /= 10000;
	d = sin(d*RAD_PER_DEG)*10000;
	bef_push(ip,(int) d);
	return EXEC_NEXTINSTR;
}

static int fixp_j(bspace *b,bip *ip,int cell)
{
	double d;
	d = (double) bef_pop(ip);
	d = asin(d/10000)*572957;
	bef_push(ip,(int) d);
	return EXEC_NEXTINSTR;
}

static int fixp_n(bspace *b,bip *ip,int cell)
{
	bef_push(ip,-bef_pop(ip));
	return EXEC_NEXTINSTR;
}

static int fixp_o(bspace *b,bip *ip,int cell)
{
	bef_push(ip,bef_pop(ip) | bef_pop(ip));
	return EXEC_NEXTINSTR;
}

static int fixp_p(bspace *b,bip *ip,int cell)
{
	bef_push(ip,(int) (((double) bef_pop(ip))*PI));
	return EXEC_NEXTINSTR;
}

static int fixp_q(bspace *b,bip *ip,int cell)
{
	cell = bef_pop(ip);
	if (cell < 0)
		bef_reflectip(b,ip);
	else
		bef_push(ip,(int) sqrt(cell));
	return EXEC_NEXTINSTR;
}

static int fixp_r(bspace *b,bip *ip,int cell)
{
	int a,bb;
	bb = bef_pop(ip);
	a = bef_pop(ip);
	bef_push(ip,(int) pow(a,bb));
	return EXEC_NEXTINSTR;
}

static int fixp_s(bspace *b,bip *ip,int cell)
{
	cell = bef_pop(ip);
	if (cell > 0)
		bef_push(ip,1);
	else if (cell < 0)
		bef_push(ip,-1);
	else
		bef_push(ip,0);
	return EXEC_NEXTINSTR;
}

static int fixp_t(bspace *b,bip *ip,int cell)
{
	double d;
	d = (double) bef_pop(ip);
	d /= 10000;
	d = tan(d*RAD_PER_DEG)*10000;
	bef_push(ip,(int) d);
	return EXEC_NEXTINSTR;
}

static int fixp_u(bspace *b,bip *ip,int cell)
{
	double d;
	d = (double) bef_pop(ip);
	d = atan(d/10000)*572957;
	bef_push(ip,(int) d);
	return EXEC_NEXTINSTR;
}

static int fixp_v(bspace *b,bip *ip,int cell)
{
	cell = bef_pop(ip);
	if (cell > 0)
		bef_push(ip,cell);
	else
		bef_push(ip,-cell);
	return EXEC_NEXTINSTR;
}

static int fixp_x(bspace *b,bip *ip,int cell)
{
	bef_push(ip,bef_pop(ip) ^ bef_pop(ip));
	return EXEC_NEXTINSTR;
}

int install_fixp(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,FIXP_FINGER,'A',fixp_a,load);
	bef_overloadsem(ip,FIXP_FINGER,'B',fixp_b,load);
	bef_overloadsem(ip,FIXP_FINGER,'C',fixp_c,load);
	bef_overloadsem(ip,FIXP_FINGER,'D',fixp_d,load);
	bef_overloadsem(ip,FIXP_FINGER,'I',fixp_i,load);
	bef_overloadsem(ip,FIXP_FINGER,'J',fixp_j,load);
	bef_overloadsem(ip,FIXP_FINGER,'N',fixp_n,load);
	bef_overloadsem(ip,FIXP_FINGER,'O',fixp_o,load);
	bef_overloadsem(ip,FIXP_FINGER,'P',fixp_p,load);
	bef_overloadsem(ip,FIXP_FINGER,'Q',fixp_q,load);
	bef_overloadsem(ip,FIXP_FINGER,'R',fixp_r,load);
	bef_overloadsem(ip,FIXP_FINGER,'S',fixp_s,load);
	bef_overloadsem(ip,FIXP_FINGER,'T',fixp_t,load);
	bef_overloadsem(ip,FIXP_FINGER,'U',fixp_u,load);
	bef_overloadsem(ip,FIXP_FINGER,'V',fixp_v,load);
	bef_overloadsem(ip,FIXP_FINGER,'X',fixp_x,load);
	return 1;
}

#endif
