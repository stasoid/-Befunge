#ifndef _SEM_ROMA_C
#define _SEM_ROMA_C

#include "roma.h"

static int roma_do(bspace *b,bip *ip,int cell)
{
	switch(cell)
	{
		case 'C': bef_push(ip,100); break;
		case 'D': bef_push(ip,500); break;
		case 'I': bef_push(ip,1); break;
		case 'L': bef_push(ip,50); break;
		case 'M': bef_push(ip,1000); break;
		case 'V': bef_push(ip,5); break;
		case 'X': bef_push(ip,10); break;
	}
	return EXEC_NEXTINSTR;
}

int install_roma(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,ROMA_FINGER,'C',roma_do,load);
	bef_overloadsem(ip,ROMA_FINGER,'D',roma_do,load);
	bef_overloadsem(ip,ROMA_FINGER,'I',roma_do,load);
	bef_overloadsem(ip,ROMA_FINGER,'L',roma_do,load);
	bef_overloadsem(ip,ROMA_FINGER,'M',roma_do,load);
	bef_overloadsem(ip,ROMA_FINGER,'V',roma_do,load);
	bef_overloadsem(ip,ROMA_FINGER,'X',roma_do,load);
	return 1;
}

#endif
