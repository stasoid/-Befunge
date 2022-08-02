#ifndef _SEM_TERM_C
#define _SEM_TERM_C

#include "term.h"

static int term_c(bspace *b,bip *ip,int cell)
{
	bef_cls(b);
	return EXEC_NEXTINSTR;
}

static int term_d(bspace *b,bip *ip,int cell)
{
	bef_cursory(b,bef_pop(ip));
	return EXEC_NEXTINSTR;
}

static int term_g(bspace *b,bip *ip,int cell)
{
	int x,y;
	y = bef_pop(ip);
	x = bef_pop(ip);
	bef_cursor(b,x,y);
	return EXEC_NEXTINSTR;
}

static int term_h(bspace *b,bip *ip,int cell)
{
	bef_cursor(b,0,0);
	return EXEC_NEXTINSTR;
}

static int term_l(bspace *b,bip *ip,int cell)
{
	bef_clearline(b);
	return EXEC_NEXTINSTR;
}

static int term_s(bspace *b,bip *ip,int cell)
{
	bef_clearscreen(b);
	return EXEC_NEXTINSTR;
}

static int term_u(bspace *b,bip *ip,int cell)
{
	bef_cursory(b,-bef_pop(ip));
	return EXEC_NEXTINSTR;
}

int install_term(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,TERM_FINGER,'C',term_c,load);
	bef_overloadsem(ip,TERM_FINGER,'D',term_d,load);
	bef_overloadsem(ip,TERM_FINGER,'G',term_g,load);
	bef_overloadsem(ip,TERM_FINGER,'H',term_h,load);
	bef_overloadsem(ip,TERM_FINGER,'L',term_l,load);
	bef_overloadsem(ip,TERM_FINGER,'S',term_s,load);
	bef_overloadsem(ip,TERM_FINGER,'U',term_u,load);
	return 1;
}

#endif
