#ifndef _SEM_DIRF_C
#define _SEM_DIRF_C

#include "dirf.h"

static int dirf_do(bspace *b,bip *ip,int cell)
{
	char tstr[256];
	int tmp;
	tmp = 0;
	switch (cell)
	{
		case 'C':
			tmp = sprintf(tstr,"dir ");
			break;
		case 'M':
			tmp = sprintf(tstr,"cdir ");
			break;
		case 'R':
			tmp = sprintf(tstr,"delete ");
			break;
	}
	do {
		if (tmp == 256)
			tmp = 255;
		tstr[tmp++] = bef_pop(ip);
	} while (tstr[tmp-1]);
	if (tmp == 256)
	{
		bef_reflectip(b,ip); /* Too long! */
		return EXEC_NEXTINSTR;
	}
	if (system(tstr) == -2)
		bef_reflectip(b,ip);
	return EXEC_NEXTINSTR;
}

int install_dirf(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,DIRF_FINGER,'C',dirf_do,load);
	bef_overloadsem(ip,DIRF_FINGER,'M',dirf_do,load);
	bef_overloadsem(ip,DIRF_FINGER,'R',dirf_do,load);
	return 1;
}

#endif
