#ifndef _SEM_CPLI_C
#define _SEM_CPLI_C

#include <math.h>
#include "cpli.h"

static int cpli_a(bspace *b,bip *ip,int cell)
{
	/* Add complex int */
	int ai,ar,bi,br;
	bi = bef_pop(ip);
	br = bef_pop(ip);
	ai = bef_pop(ip);
	ar = bef_pop(ip);
	bef_push(ip,ar+br);
	bef_push(ip,ai+bi);
	return EXEC_NEXTINSTR;
}

static int cpli_d(bspace *b,bip *ip,int cell)
{
	/* Divide complex int */
	int ai,ar,bi,br;
	bi = bef_pop(ip);
	br = bef_pop(ip);
	ai = bef_pop(ip);
	ar = bef_pop(ip);
	bef_push(ip,(ar*br+ai*bi)/(br*br+bi*bi));
	bef_push(ip,(ai*br-ar*bi)/(br*br+bi*bi));
	return EXEC_NEXTINSTR;
}

static int cpli_m(bspace *b,bip *ip,int cell)
{
	/* Multiply complex int */
	int ai,ar,bi,br;
	bi = bef_pop(ip);
	br = bef_pop(ip);
	ai = bef_pop(ip);
	ar = bef_pop(ip);
	bef_push(ip,ar*br-ai*bi);
	bef_push(ip,ar*bi+ai*br);
	return EXEC_NEXTINSTR;
}

static int cpli_o(bspace *b,bip *ip,int cell)
{
	/* Output complex int */
	int ai,ar;
	char tstr[32];
	ai = bef_pop(ip);
	ar = bef_pop(ip);
	sprintf(tstr,"%d%+di ",ar,ai);
	if (bef_puts(b,tstr) == EOF)
		bef_reflectip(b,ip);
	return EXEC_NEXTINSTR;
}

static int cpli_s(bspace *b,bip *ip,int cell)
{
	/* Subtract complex int */
	int ai,ar,bi,br;
	bi = bef_pop(ip);
	br = bef_pop(ip);
	ai = bef_pop(ip);
	ar = bef_pop(ip);
	bef_push(ip,ar-br);
	bef_push(ip,ai-bi);
	return EXEC_NEXTINSTR;
}

static int cpli_v(bspace *b,bip *ip,int cell)
{
	/* Absolute value of complex int */
	int ai,ar;
	ai = bef_pop(ip);
	ar = bef_pop(ip);
	bef_push(ip,(int) sqrt(ai*ai+ar*ar));
	return EXEC_NEXTINSTR;
}

int install_cpli(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,CPLI_FINGER,'A',cpli_a,load);
	bef_overloadsem(ip,CPLI_FINGER,'D',cpli_d,load);
	bef_overloadsem(ip,CPLI_FINGER,'M',cpli_m,load);
	bef_overloadsem(ip,CPLI_FINGER,'O',cpli_o,load);
	bef_overloadsem(ip,CPLI_FINGER,'S',cpli_s,load);
	bef_overloadsem(ip,CPLI_FINGER,'V',cpli_v,load);
	return 1;
}

#endif
