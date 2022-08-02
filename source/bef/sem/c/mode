#ifndef _SEM_MODE_C
#define _SEM_MODE_C

#include <math.h>

#include "mode.h"

static int mode_do(bspace *b,bip *ip,int cell)
{
	if (cell == 'H')
		ip->flags ^= IP_HOVERMODE;
	if (cell == 'I')
		ip->flags ^= IP_INVERTMODE;
	if (cell == 'Q')
		ip->flags ^= IP_QUEUEMODE;
	if (cell == 'S')
		ip->flags ^= IP_SWITCHMODE;
	return EXEC_NEXTINSTR;
}

int install_mode(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,MODE_FINGER,'H',mode_do,load);
	bef_overloadsem(ip,MODE_FINGER,'I',mode_do,load);
	bef_overloadsem(ip,MODE_FINGER,'Q',mode_do,load);
	bef_overloadsem(ip,MODE_FINGER,'S',mode_do,load);
	return 1;
}

#endif
