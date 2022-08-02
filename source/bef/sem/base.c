#ifndef _SEM_BASE_C
#define _SEM_BASE_C

#include "base.h"

static int base_do(bspace *b,bip *ip,int cell)
{
	char tstr[36];
	int tmp,base,tmp2;
	switch(cell)
	{
		case 'B':
			base = 2;
			cell = bef_pop(ip);
			break;
		case 'H':
			sprintf(tstr,"%x ",(unsigned int) bef_pop(ip));
			if (bef_puts(b,tstr) == EOF)
				bef_reflectip(b,ip);
			return EXEC_NEXTINSTR;
		case 'I':
			base = bef_pop(ip);
			if ((base < 2) || (base > 36))
			{
				bef_reflectip(b,ip);
				return EXEC_NEXTINSTR;
			}
			tmp = bef_getint(b,ip,base,&tmp2);
			if (tmp2)
			{
				bef_push(ip,base);
				return EXEC_WAIT;
			}
			if (tmp == EOF)
				bef_reflectip(b,ip);
			else
				bef_push(ip,tmp);
			return EXEC_NEXTINSTR;
		case 'N':
			base = bef_pop(ip);
			cell = bef_pop(ip);
			if ((base < 2) || (base > 36))
			{
				bef_reflectip(b,ip);
				return EXEC_NEXTINSTR;
			}
			break;
		case 'O':
			sprintf(tstr,"%o ",(unsigned int) bef_pop(ip));
			if (bef_puts(b,tstr) == EOF)
				bef_reflectip(b,ip);
			return EXEC_NEXTINSTR;
		default:
			return EXEC_NEXTINSTR;
	}
	if (cell < 0)
	{
		if (base == 2)
		{
			tmp = '1';
			cell = cell & 0x7FFFFFFF;
		}
		else
		{
			cell = -cell;
			tmp = '-';
		}
		if (bef_putchar(b,tmp) == EOF)
		{
			bef_reflectip(b,ip);
			return EXEC_NEXTINSTR;
		}
	}
	tmp2 = 1;
	tstr[0] = ' ';
	while (cell >= 0)
	{
		tmp = cell % base;
		if (tmp < 10)
			tstr[tmp2++] = '0'+tmp;
		else
			tstr[tmp2++] = 'a'+tmp;
		cell = cell / base;
	}
	while (tmp2 > 0)
		if (bef_putchar(b,tstr[--tmp2]) == EOF)
		{
			bef_reflectip(b,ip);
			return EXEC_NEXTINSTR;
		}
	return EXEC_NEXTINSTR;
}

int install_base(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,BASE_FINGER,'B',base_do,load);
	bef_overloadsem(ip,BASE_FINGER,'H',base_do,load);
	bef_overloadsem(ip,BASE_FINGER,'I',base_do,load);
	bef_overloadsem(ip,BASE_FINGER,'N',base_do,load);
	bef_overloadsem(ip,BASE_FINGER,'O',base_do,load);
	return 1;
}

#endif
