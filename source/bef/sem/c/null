#ifndef _SEM_NULL_C
#define _SEM_NULL_C

#include "null.h"

static int null_reflect(bspace *b,bip *ip,int cell)
{
	bef_reflectip(b,ip);
	return EXEC_NEXTINSTR;
}

int install_null(bspace *b,bip *ip,int load)
{
	int a;
	for (a=0;a<26;a++)
		bef_overloadsem(ip,NULL_FINGER,a+'A',null_reflect,load);
	return 1;
}

#endif
