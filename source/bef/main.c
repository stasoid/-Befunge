#ifndef _BEF_MAIN_C
#define _BEF_MAIN_C

#include "main.h"
#include "io.h"
#include "ip.h"
#include "mem.h"
#include "prog.h"
#include "sem.h"
#include "stack.h"
#include "../version.h"
#include "dynafing.h"

#ifdef LYT
#include "../lyt.h"
#endif

int ip_refresh;

/* Execution... */

void bef_reflectip(bspace *b,bip *ip)
{
	int tmp;
	for (tmp=0;tmp<b->dims;tmp++)
		ip->delta[tmp] = -ip->delta[tmp];
}

int bef_inspace(bspace *b,int *pos)
{
	/* Return 1 if the pos is inside space */
	/* i.e. if constrained dimension inside constraints */
	/* else inside minmem,maxmem */
	int tmp;
	for (tmp=0;tmp<b->dims;tmp++)
	{
		if (b->sizes[tmp])
		{
			if ((unsigned long) pos[tmp] >= b->sizes[tmp])
				return 0; /* Outside */
		}
		else if ((pos[tmp] < b->minmem[tmp]) || (pos[tmp] > b->maxmem[tmp]))
			return 0;
	}
	return 1;
}

void bef_nextpos(bspace *b,bip *ip,int bigskip)
{
	int tmp,tmp2,tmp3;
	tmp3=0;
	for (tmp=0;tmp<b->dims;tmp++)
	{
		ip->pos[tmp] += ip->delta[tmp];
		if (ip->delta[tmp]) /* Only wrap address if we are moving in that dir */
		{
			if (b->sizes[tmp] != 0)
			{
				if (((unsigned long) ip->pos[tmp]) >= b->sizes[tmp])
				{
					ip->pos[tmp] = ip->pos[tmp] % ((int) b->sizes[tmp]);
					if (ip->pos[tmp] < 0)
						ip->pos[tmp] += b->sizes[tmp];
				}
			}
			else if (bigskip)
			{
				if ((ip->flags & IP_FLYING) == 0)
				{ /* Only if travelling cardinally */
					if (ip->delta[tmp] > 0)
						tmp2 = 1;
					else
						tmp2 = 0;
					tmp2 = (int) bef_getmemspace(b,ip->pos,tmp,tmp2); /* Get distance to next cell which might be occupied */
					if (tmp2 != -1)
						ip->pos[tmp] += ip->delta[tmp]*tmp2; /* Move there */
					else
						ip->pos[tmp] -= ip->delta[tmp]; /* Backtrack to old pos */
				}
				else if ((ip->pos[tmp] < b->minmem[tmp]) || (ip->pos[tmp] > b->maxmem[tmp]))
					tmp3 = 1; /* Backtrack */
			}
		}
	}
	if (tmp3 == 0)
		return;
	/* ... else backtrack until other end of level reached */
	bef_reflectip(b,ip);
	do { /* Move till inside */
		bef_nextpos(b,ip,0); /* Recurse a bit, making sure not to do it infinitely */
	} while (bef_inspace(b,ip->pos) == 0);
	do { /* Move till outside */
		bef_nextpos(b,ip,0);
	} while (bef_inspace(b,ip->pos) == 1);
	bef_reflectip(b,ip);
	bef_nextpos(b,ip,0); /* Move back inside */
}

void bef_y(bspace *b,bip *ip)
{
	/* Perform the y-func */
	int y,ss,tmp,tmp2;
	char *a;
	struct tm *thetime;
	time_t atime;
	y = bef_pop(ip); /* Get y-value */
	ss = bef_stacksize(ip,0); /* Size of current stack */
	/* 20: Environment variables */
	bef_push(ip,0);
	/* 19: Command line args */
	bef_push(ip,0); /* Output double null */
	bef_push(ip,0);
	/* Push the args string in reverse order, replacing spaces with nulls */
	if (b->args[0])
	{
		a = b->args;
		while (*a) /* Go through string and reverse escaped chars */
		{
			if (*a == '\\') /* Escaped char? */
			{
				*a = *(a+1);
				*(a+1) = '\\';
				a++;
			}
			a++;
		}
		a--;
		/* Now output them */
		tmp = 1; /* Output null if char found */
		while (a >= b->args)
		{
			if ((*a != 32) || (tmp == 2)) /* Not space or in string */
			{
				if (tmp == 1) /* Need to output null? */
				{
					bef_push(ip,0);
					tmp = 0;
				}
				if (*a == '"') /* String? */
				{
					if (tmp == 0)
						tmp = 2;
					else
						tmp = 1; /* Is this right? */
				}
				else if (*a == '\\') /* Escaped char? */
				{
					bef_push(ip,*(a-1));
					*a = *(a-1); /* Restore original direction */
					*(a-1) = '\\';
					a--;
				}
				else
					bef_push(ip,*a);
			}
			else
				tmp = 1; /* Output null at next char */
			a--;
		}
	}
	bef_pushstring(ip,b->fname); /* Push file name as arg 0 */
	/* 18: Stack sizes */
	tmp = tmp2 = bef_countstacks(ip);
	while (tmp-- > 1)
		bef_push(ip,bef_stacksize(ip,tmp));
	bef_push(ip,ss); /* Size of stack 0 */
	/* 17: Number of stacks */
	bef_push(ip,tmp2);
	/* 16: Current time */
	atime = time(NULL);
	thetime = localtime(&atime);
	bef_push(ip,(thetime->tm_hour*65536)+(thetime->tm_min*256)+thetime->tm_sec);
	/* 15: Current date */
	bef_push(ip,(thetime->tm_year*65536)+(thetime->tm_mon*256)+thetime->tm_mday);
	/* 14: Greatest non-cell pos, relative to least */
	for (tmp=0;tmp<b->dims;tmp++)
		bef_push(ip,b->maxmem[tmp]-b->minmem[tmp]);
	/* 13: Least non-cell pos */
	for (tmp=0;tmp<b->dims;tmp++)
		bef_push(ip,b->minmem[tmp]);
	/* 12: Storage offset */
	for (tmp=0;tmp<b->dims;tmp++)
		bef_push(ip,ip->origin[tmp]);
	/* 11: Delta */
	for (tmp=0;tmp<b->dims;tmp++)
		bef_push(ip,ip->delta[tmp]);
	/* 10: Pos */
	for (tmp=0;tmp<b->dims;tmp++)
		bef_push(ip,ip->pos[tmp]);
	/* 9: IP team */
	bef_push(ip,0); /* No teams (yet!) */
	/* 8: IP id */
	bef_push(ip,ip->id);
	/* 7: No. of dims */
	bef_push(ip,b->dims);
	/* 6: Path seperator */
#ifndef LYT
	bef_push(ip,'.'); /* Presumably good programs will spot this and not try using PC file extensions */
#else
#ifndef PATH_SEPERATOR
#ifdef __riscos__
#define PATH_SEPERATOR '.'
#else
#ifdef __MSDOS__
#define PATH_SEPERATOR '\\'
#else
#define PATH_SEPERATOR '/'
#endif
#endif
#endif
	bef_push(ip,PATH_SEPERATOR);
#endif
	/* 5: Operating paradigm */
	if (b->flags & PROG_FS)
		bef_push(ip,1); /* Like C */
	else
		bef_push(ip,0); /* Unavailable */
	/* 4: Interpreter version */
	bef_push(ip,(int) (VERSION*100));
	/* 3: Interpreter handprint */
	bef_push(ip,HANDPRINT);
	/* 2: Bytes per cell */
	bef_push(ip,b->bits/8);
	/* 1: Flags */
	tmp = 0;
	if (b->flags & PROG_CON)
		tmp |= 1;
	if (b->flags & PROG_FS)
		tmp |= 2+4+8;
	/* Need option to control IO buffering... */
	bef_push(ip,tmp);
	/* Now handle y-value */
	/* If <= 0, do nothing */
	/* Else return just bef_peekstack(ip,y-1) */
	if (y > 0)
	{
		tmp = bef_peekstack(ip,y-1);
		tmp2 = bef_stacksize(ip,0);
		while (tmp2-- > ss)
			bef_pop(ip); /* Save time by not calling stacksize each loop */
		bef_push(ip,tmp);
	}
}

static void trace_ip(bspace *b,bip *ip,int cell)
{
	int tmp,tmp2,tmp3;
	char *delplus,*delminus;
	if (UP_DIR(b) == -1)
	{
		delplus = ">vl";
		delminus = "<^h";
	}
	else
	{
		delplus = ">vh";
		delminus = "<^l";
	}
	printf("IP %d at (%d",ip->id,ip->pos[0]);
	for (tmp=1;tmp<b->dims;tmp++)
		printf(",%d",ip->pos[tmp]);
	printf(") ");
	for (tmp=0;tmp<b->dims;tmp++)
		if (ip->delta[tmp] > 0)
			printf("%c",delplus[tmp]);
		else if (ip->delta[tmp] < 0)
			printf("%c",delminus[tmp]);
	if (ip->flags & IP_STRINGMODE)
		printf("\"");
	else if (b->input_ip == ip)
		printf("i");
	if (ip->flags & IP_HOVERMODE)
		printf("H");
	if (ip->flags & IP_INVERTMODE)
		printf("I");
	if (ip->flags & IP_QUEUEMODE)
		printf("Q");
	if (ip->flags & IP_SWITCHMODE)
		printf("S");
	if (b->flags & PROG_FINGERPRINT)
		printf("F"); /* IP is in a fingerprint */
	if ((ip->ghost) && (ip->ghost->id > ip->id))
		printf("G"); /* IP has a ghost */
	if (ip->flags & IP_DEAD)
		printf("D");
	printf(" org. (%d",ip->origin[0]);
	for (tmp=1;tmp<b->dims;tmp++)
		printf(",%d",ip->origin[tmp]);
	printf(") over ");
	if ((cell > 31) && (cell < 256) && (cell != 127))
		printf("'%c'",cell);
	else
		printf("0x%x",cell);
	printf(", stk:");
	tmp3 = bef_stacksize(ip,0);
	for (tmp=0;(tmp<tmp3) && (tmp<10);tmp++)
	{
		tmp2 = bef_peekstack(ip,tmp);
		printf(" %d",tmp2);
		if ((tmp2 > 31) && (tmp2 < 256) && (tmp2 != 127))
			printf("('%c')",tmp2);
	}
	if (tmp3 > 10)
		printf(" ...");
	printf("\n");
}

void bef_unimplemented(bspace *b,bip *ip,int cell)
{
	char tstr[256],*n;
	int tmp;
	if (b->flags & PROG_WARN_UNIMPLEMENTED)
	{
		n = tstr;
		n+= sprintf(n,"IP #%d tried to execute %d 0x%x",ip->id,cell,cell);
		if ((cell > 31) && (cell < 256) && (cell != 127))
			n += sprintf(n," '%c'",cell);
		/* Show pos */
		n += sprintf(n," at (%d",ip->pos[0]);
		for (tmp=1;tmp<b->dims;tmp++)
			n += sprintf(n,",%d",ip->pos[tmp]);
		n += sprintf(n,")");
		interface_statusalert(b,tstr,3); /* Do in singletasking & wait for response */
	}
	bef_reflectip(b,ip);
}

int bef_checkbreakpoints(bspace *b)
{
	/* Check breakpoints for all IPs */
	/* Return 1 if OK to go on, or 0 if breakpoint hit */
	int tmp,tmp2,tmp3;
	bip *ip,*nip;
	char tstr[256];
	ip = b->ips;
	while (ip)
	{
		nip = ip->next;
		while (ip->ghost)
			ip = ip->ghost;
		if (ip->space->breakflags != 0)
		{
			tmp2 = 0;
			while ((1 << tmp2) <= ip->space->breakflags)
			{
				if (ip->space->breakflags & (1 << tmp2))
				{
					tmp = 0;
					for (tmp3=0;tmp3<ip->space->dims;tmp3++)
						if (ip->space->breaks[tmp2][tmp3] != ip->pos[tmp3])
							tmp = -1;
					if (tmp != -1)
					{
						sprintf(tstr,"%sIP #%d hit breakpoint %d",(ip->haunted ? "Fingerprint " : ""),ip->id,tmp2+1);
						interface_statusalert(b,tstr,3);
						return 0;
					}
				}
				tmp2++;
			}
		}
		ip = nip;
	}
	return 1;
}

void bef_flycheck(bspace *b,bip *ip)
{
	int tmp,tmp2;
	tmp2=0;
	for (tmp=0;tmp<b->dims;tmp++)
		if (ip->delta[tmp] > 0)
			tmp2 += ip->delta[tmp]; /* Get sum of absolute values */
		else
			tmp2 -= ip->delta[tmp];
	if (tmp2 > 1)
		ip->flags |= IP_FLYING;
	else
		ip->flags &= ~IP_FLYING;
}

int bef_execinstr(bspace *b,bip *ip,int cell)
{
	/* Execute the given instruction & return without moving to next instr */
	int tmp,tmp2,tmp3;
	int tpos[MAX_DIM],tpos2[MAX_DIM];
	signed long *tstck;
	char tstr[512];
	bip *nip;
	if ((ip->flags & IP_DEAD) && (ip->ghost == NULL))
	{
		bef_killip(b,ip);
		return EXEC_DEAD;
	}
	switch (cell)
	{
		case ' ': /* Space */
			break;
		case '!': /* Logical not */
			if (bef_pop(ip) == 0)
				bef_push(ip,1);
			else
				bef_push(ip,0);
			break;
		case '"': /* Toggle stringmode */
			ip->flags |= IP_STRINGMODE;
			bef_nextpos(b,ip,0);
			if (ip_refresh)
				interface_updatespace(b,ip->pos,0);
			return EXEC_IMADEMOVE;
		case '#': /* Trampoline */
			bef_nextpos(b,ip,0);
			break;
		case '$': /* Pop */
			bef_pop(ip);
			break;
		case '%': /* Remainder */
			tmp2 = bef_pop(ip);
			tmp = bef_pop(ip);
			if (tmp2 == 0)
			{
				tmp3 = bef_query(b,ip,tmp,cell,&tmp2);
				if (tmp2 == EOF) /* Error */
					bef_reflectip(b,ip);
				else if (tmp2 == 1) /* Wait for result */
				{
					bef_push(ip,tmp); /* Push number back on */
					bef_push(ip,0); /* The zero */
					return EXEC_WAIT; /* Wait */
				}
				else
					bef_push(ip,tmp3); /* Got result */
			}
			else
				bef_push(ip,tmp % tmp2);
			break;
		case '&': /* Input integer */
			tmp = bef_getint(b,ip,10,&tmp2);
			if (tmp2) /* Need to wait */
				return EXEC_WAIT;
			if (tmp == EOF)
				bef_reflectip(b,ip); /* Error */
			else
				bef_push(ip,tmp);
			break;
		case 39: /* Fetch character (98) */
			if (b->flags & PROG_98)
			{
				bef_nextpos(b,ip,0);
				bef_push(ip,bef_readspace(b,ip->pos,(int *) NULL));
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case '(': /* Load semantics (98) */
			if ((b->flags & PROG_98) && ((b->flags & PROG_MINIFUNGE) == 0))
				bef_loadsem(b,ip);
			else
				bef_unimplemented(b,ip,cell);
			if (ip->flags & IP_SWITCHMODE)
				bef_setspace(b,ip->pos,(int *) NULL,')');
			break;
		case ')': /* Unload semantics (98) */
			if ((b->flags & PROG_98) && ((b->flags & PROG_MINIFUNGE) == 0))
				bef_unloadsem(b,ip);
			else
				bef_unimplemented(b,ip,cell);
			if (ip->flags & IP_SWITCHMODE)
				bef_setspace(b,ip->pos,(int *) NULL,'(');
			break;
		case '*': /* Multiply */
			bef_push(ip,bef_pop(ip)*bef_pop(ip));
			break;
		case '+': /* Add */
			bef_push(ip,bef_pop(ip)+bef_pop(ip));
			break;
		case ',': /* Output char */
			if (bef_putchar(b,bef_pop(ip)) == EOF)
				bef_reflectip(b,ip);
			break;
		case '-': /* Subtract */
			tmp2 = bef_pop(ip);
			tmp = bef_pop(ip);
			bef_push(ip,tmp-tmp2);
			break;
		case '.': /* Output int */
			sprintf(tstr,"%d ",(int) bef_pop(ip));
			if (bef_puts(b,tstr) == EOF)
				bef_reflectip(b,ip);
			break;
		case '/': /* Divide */
			tmp2 = bef_pop(ip);
			tmp = bef_pop(ip);
			if (tmp2 == 0)
			{
				tmp3 = bef_query(b,ip,tmp,cell,&tmp2);
				if (tmp2 == EOF) /* Error */
					bef_reflectip(b,ip);
				else if (tmp2 == 1) /* Wait for result */
				{
					bef_push(ip,tmp); /* Push number back on */
					bef_push(ip,0); /* The zero */
					return EXEC_WAIT; /* Wait */
				}
				else
					bef_push(ip,tmp3); /* Got result */
			}
			else
				bef_push(ip,tmp/tmp2);
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			bef_push(ip,cell-'0');
			break;
		case ':': /* Duplicate */
			tmp = bef_pop(ip);
			bef_push(ip,tmp);
			bef_push(ip,tmp);
			break;
		case ';': /* Jump over (98) */
			if (b->flags & PROG_98)
				break; /* This is handled by code below, since it's meant to take 0 time */
			else
				bef_unimplemented(b,ip,cell);
			break;
		case '<': /* Go west */
			if ((ip->flags & IP_HOVERMODE) == 0)
				for (tmp=0;tmp<b->dims;tmp++)
					ip->delta[tmp] = 0;
			ip->delta[0]--;
			bef_flycheck(b,ip);
			break;
		case '=': /* Execute (98,filesystem) */
			if ((b->flags & PROG_98) && (b->flags & PROG_FS))
			{
				/* Get command */
				tmp2 = 0;
				do {
					if (tmp2 == 512)
						tmp2 = 511;
					tstr[tmp2++] = bef_pop(ip);
				} while (tstr[tmp2-1] != 0);
				if (tmp2 == 512)
					bef_push(ip,-2); /* Buffer overrun */
				else if (nowimp)
					bef_push(ip,system(tstr));
#ifndef LYT
				else
				{
					if (LimpX_StartTask(tstr))
						bef_push(ip,0); /* Assume OK if WIMP app */
					else if (atoi((getenv("Sys$ReturnCode") ? : "1")) != 0) /* Return code of 1 if return code not found, should never happen though */
						bef_push(ip,-2); /* system() only returns 0 or -2 apparently */
					else
						bef_push(ip,0);
				}
#endif
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case '>': /* Go east */
			if ((ip->flags & IP_HOVERMODE) == 0)
				for (tmp=0;tmp<b->dims;tmp++)
					ip->delta[tmp] = 0;
			ip->delta[0]++;
			bef_flycheck(b,ip);
			break;
		case '?': /* Go away */
			for (tmp=0;tmp<b->dims;tmp++)
				ip->delta[tmp] = 0;
			if (rand() >= RAND_MAX/2)
				tmp2 = -1;
			else
				tmp2 = 1;
			tmp = (int) ((((float) rand())/RAND_MAX)*((float) b->dims));
			ip->delta[tmp] = tmp2;
			ip->flags &= ~IP_FLYING;
			break;
		case '@': /* Stop */
			ip->flags |= IP_DEAD;
			dynafing_kill(b,ip);
			if (ip->ghost) /* Still have active ghost */
				return EXEC_WAIT;
			bef_killip(b,ip); /* ... else die */
			return EXEC_DEAD;
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
		case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
		case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
		case 'Y': case 'Z': /* Fingerprint (98) */
			if (b->flags & PROG_98)
				return bef_callsem(b,ip,cell);
			else
				bef_unimplemented(b,ip,cell);
			break;
		case '[': /* Turn left (98) */
			if ((b->flags & PROG_98) && (b->dims > 1))
			{
				tmp = ip->delta[0];
				ip->delta[0] = ip->delta[1];
				ip->delta[1] = -tmp;
			}
			else
				bef_unimplemented(b,ip,cell);
			if (ip->flags & IP_SWITCHMODE)
				bef_setspace(b,ip->pos,(int *) NULL,']');
			break;
		case 92: /* Swap */
			tmp2 = bef_pop(ip);
			tmp = bef_pop(ip);
			bef_push(ip,tmp2);
			bef_push(ip,tmp);
			break;
		case ']': /* Turn right (98) */
			if ((b->flags & PROG_98) && (b->dims > 1))
			{
				tmp = ip->delta[0];
				ip->delta[0] = -ip->delta[1];
				ip->delta[1] = tmp;
			}
			else
				bef_unimplemented(b,ip,cell);
			if (ip->flags & IP_SWITCHMODE)
				bef_setspace(b,ip->pos,(int *) NULL,'[');
			break;
		case '^': /* Go north */
			if (b->dims > 1)
			{
				if ((ip->flags & IP_HOVERMODE) == 0)
					for (tmp=0;tmp<b->dims;tmp++)
						ip->delta[tmp] = 0;
				ip->delta[1]--;
				bef_flycheck(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case '_': /* East-west if */
			if ((ip->flags & IP_HOVERMODE) == 0)
				for (tmp=0;tmp<b->dims;tmp++)
					ip->delta[tmp] = 0;
			if (bef_pop(ip) == 0)
				ip->delta[0]++;
			else
				ip->delta[0]--;
			bef_flycheck(b,ip);
			break;
		case '`': /* Greater than */
			tmp2 = bef_pop(ip);
			tmp = bef_pop(ip);
			if (tmp > tmp2)
				bef_push(ip,1);
			else
				bef_push(ip,0);
			break;
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': /* 98 */
			if (b->flags & PROG_98)
				bef_push(ip,(cell-'a')+10);
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'g': /* Get */
			for (tmp=b->dims-1;tmp>=0;tmp--)
				tpos[tmp] = bef_pop(ip);
			if (b->flags & PROG_MINIFUNGE)
				bef_push(ip,bef_readspace(b->haunted,tpos,ip->haunted->origin));
			else
				bef_push(ip,bef_readspace(b,tpos,ip->origin));
			break;
		case 'h': /* Go high (98) */
			if ((b->flags & PROG_98) && (b->dims > 2))
			{
				if ((ip->flags & IP_HOVERMODE) == 0)
					for (tmp=0;tmp<b->dims;tmp++)
						ip->delta[tmp] = 0;
				ip->delta[2] += UP_DIR(b);
				bef_flycheck(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'i': /* Input file (98, filesystem) */
			if ((b->flags & PROG_98) && (b->flags & PROG_FS))
			{
				tmp=0;
				do {
					if (tmp == 512)
						tmp = 511;
					tstr[tmp++] = bef_pop(ip);
				} while (tstr[tmp-1] != 0);
				tmp = bef_pop(ip); /* Flags */
				for (tmp2=b->dims-1;tmp2>=0;tmp2--)
					tpos[tmp2] = bef_pop(ip); /* Location */
				if (tmp == 512) /* Buffer overrun */
					bef_reflectip(b,ip);
				else if (load_file(b,tstr,tpos,tmp,tpos2) == EOF)
					bef_reflectip(b,ip);
				else /* Now push pos & size vectors */
				{
					for (tmp2=0;tmp2<b->dims;tmp2++)
						bef_push(ip,tpos2[tmp2]);
					for (tmp2=0;tmp2<b->dims;tmp2++)
						bef_push(ip,tpos[tmp2]);
				}
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'j': /* Jump forward (98) */
			if (b->flags & PROG_98)
			{
				tmp2 = bef_pop(ip);
				if (tmp2 < 0) /* Handle negative no's */
				{
					bef_reflectip(b,ip);
					tmp = 1;
					tmp2 = -tmp2;
				}
				else
					tmp = 0;
				while (tmp2 > 0)
				{
					bef_nextpos(b,ip,0);
					tmp2--;
				}
				if (tmp)
					bef_reflectip(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'k': /* Iterate (98) */
			if (b->flags & PROG_98)
			{
				/* According to spec, we need to move on to the next executable instruction */
				/* Then do it 'n' times */
				/* Then continue as normal (i.e. from k+delta) */
				/* so 0k will just run the instruction once */
				/* Nasty spec suggests that 0k^v will go down! */
				/* And FBBI will iterate over negative n's */
				tmp = bef_pop(ip);
				for (tmp2=0;tmp2<b->dims;tmp2++)
					tpos[tmp2] = ip->pos[tmp2];
				bef_nextinstr(b,ip,cell); /* Find next executable */
				tmp2 = bef_readspace(b,ip->pos,(int *) NULL); /* Get instr */
				for (tmp3=0;tmp3<b->dims;tmp3++)
					ip->pos[tmp3] = tpos[tmp3]; /* Move back */
				tmp3 = 0;
				while (tmp > 0)
				{
					switch (bef_execinstr(b,ip,tmp2))
					{
						case EXEC_DEAD:
							return EXEC_DEAD;
						case EXEC_GHOSTSPAWN:
							tmp3 = 1;
						case EXEC_WAIT:
							/* IP is waiting for input */
							/* Push a count back onto the stack and exit then */
							bef_push(ip,tmp);
							return EXEC_WAIT;
					}
					tmp--; /* ... else OK to continue (imademove/nextinstr) */
				}
				if (tmp3)
					return EXEC_GHOSTSPAWN;
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'l': /* Go low (98) */
			if ((b->flags & PROG_98) && (b->dims > 2))
			{
				if ((ip->flags & IP_HOVERMODE) == 0)
					for (tmp=0;tmp<b->dims;tmp++)
						ip->delta[tmp] = 0;
				ip->delta[2] += DOWN_DIR(b);
				bef_flycheck(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'm': /* High-low if (98) */
			if ((b->flags & PROG_98) && (b->dims > 2))
			{
				if ((ip->flags & IP_HOVERMODE) == 0)
					for (tmp=0;tmp<b->dims;tmp++)
						ip->delta[tmp] = 0;
				if (bef_pop(ip) == 0)
					ip->delta[2] += DOWN_DIR(b);
				else
					ip->delta[2] += UP_DIR(b);
				bef_flycheck(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'n': /* Clear stack (98) */
			if (b->flags & PROG_98)
			{
				tmp = bef_stacksize(ip,0);
				while (tmp--)
					bef_pop(ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'o': /* Output file (98, filesystem) */
			if ((b->flags & PROG_98) && (b->flags & PROG_FS))
			{
				tmp=0;
				do {
					if (tmp == 512)
						tmp = 511;
					tstr[tmp++] = bef_pop(ip);
				} while (tstr[tmp-1] != 0);
				tmp = bef_pop(ip); /* Flags */
				for (tmp2=b->dims-1;tmp2>=0;tmp2--)
					tpos[tmp2] = bef_pop(ip); /* Location */
				for (tmp2=b->dims-1;tmp2>=0;tmp2--)
					tpos2[tmp2] = bef_pop(ip); /* Size */
				if (tmp == 512)
					bef_reflectip(b,ip);
				else if (save_file(b,tstr,tpos,tpos2,tmp) == EOF)
					bef_reflectip(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'p': /* Put */
			for (tmp=b->dims-1;tmp>=0;tmp--)
				tpos[tmp] = bef_pop(ip);
			if (b->flags & PROG_MINIFUNGE)
				bef_setspace(b->haunted,tpos,ip->haunted->origin,bef_pop(ip));
			else
				bef_setspace(b,tpos,ip->origin,bef_pop(ip));
			break;
		case 'q': /* Quit (98) */
			if ((b->flags & PROG_98) && ((b->flags & PROG_MINIFUNGE) == 0))
			{
				if (b->flags & PROG_FINGERPRINT)
				{
					/* Fingerprint-q */
					ip->flags |= IP_DEAD;
					return bef_execinstr(ip->haunted->space,ip->haunted,'@');
				}
				tmp = bef_pop(ip);
				while (b->ips != NULL)
					bef_killip(b,b->ips); /* Kill all ip's without notifying them */
				if (nowimp)
					bef_exit(tmp); /* This won't return */
				/* ... else display as text */
				sprintf(tstr,"Program exited with code %d",tmp);
				interface_statusalert(b,tstr,0);
				return EXEC_DEAD;
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'r': /* Reflect (98) */
			if (b->flags & PROG_98)
				bef_reflectip(b,ip);
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 's': /* Store char (98) */
			if (b->flags & PROG_98)
			{
				bef_nextpos(b,ip,0);
				bef_setspace(b,ip->pos,(int *) NULL,bef_pop(ip));
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 't': /* Split (98, concurrent) */
			if ((b->flags & PROG_98) && (b->flags & PROG_CON))
			{
				nip = bef_newip(b,ip);
				for (tmp=0;tmp<b->dims;tmp++)
					nip->delta[tmp] *= -1;
				if (nip->ghost == NULL)
					bef_nextpos(b,nip,0); /* Make sure it goes past the 't'! */
				interface_updatespace(b,nip->pos,(int *) NULL);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'u': /* Stack under stack (98) */
			if ((b->flags & PROG_98) && ((b->flags & PROG_MINIFUNGE) == 0))
			{
				if (bef_stackstackempty(ip))
					bef_reflectip(b,ip);
				else
				{
					tmp = bef_pop(ip);
					while (tmp > 0)
					{
						bef_push(ip,bef_underpop(ip));
						tmp--;
					}
					while (tmp < 0)
					{
						bef_underpush(ip,bef_pop(ip));
						tmp++;
					}
				}
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'v': /* Go south */
			if (b->dims > 1)
			{
				if ((ip->flags & IP_HOVERMODE) == 0)
					for (tmp=0;tmp<b->dims;tmp++)
						ip->delta[tmp] = 0;
				ip->delta[1]++;
				bef_flycheck(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'w': /* Compare (98) */
			if ((b->flags & PROG_98) && (b->dims > 1))
			{
				tmp2 = bef_pop(ip);
				tmp = bef_pop(ip);
				if (tmp > tmp2)
				{
					tmp = ip->delta[0];
					ip->delta[0] = -ip->delta[1];
					ip->delta[1] = tmp;
				}
				else if (tmp < tmp2)
				{
					tmp = ip->delta[0];
					ip->delta[0] = ip->delta[1];
					ip->delta[1] = -tmp;
				}
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'x': /* Absolute delta (98) */
			if ((b->flags & PROG_98) && ((b->flags & PROG_MINIFUNGE) == 0))
			{
				for (tmp=b->dims-1;tmp>=0;tmp--)
					ip->delta[tmp] = bef_pop(ip);
				bef_flycheck(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'y': /* Get sys info (98) */
			if (b->flags & PROG_98)
			{
				if (b->flags & PROG_MINIFUNGE)
				{
					dynafing_stacktransplant(ip,ip->haunted);
					bef_y(b->haunted,ip->haunted);
					dynafing_stacktransplant(ip,ip->haunted);
				}
				else
					bef_y(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case 'z': /* No-op (98) */
			if ((b->flags & PROG_98) == 0)
				bef_unimplemented(b,ip,cell);
			break;
		case '{': /* Begin block (98) */
			if ((b->flags & PROG_98) && ((b->flags & PROG_MINIFUNGE) == 0))
			{
				tmp = bef_pop(ip);
				bef_pushstack(ip);
				if (tmp > 0)
				{
					tstck = malloc(sizeof(signed long)*tmp); /* Just use a temp array for now */
					for (tmp2=0;tmp2<tmp;tmp2++)
						tstck[tmp2] = bef_underpop(ip);
					/* Now write back... */
					while (tmp > 0)
						bef_push(ip,tstck[--tmp]);
					free(tstck);
				}
				while (tmp < 0)
				{
					tmp++;
					bef_underpush(ip,0); /* Push onto stack underneath */
				}
				/* Push storage offset */
				for (tmp=0;tmp<b->dims;tmp++)
					bef_underpush(ip,ip->origin[tmp]);
				/* Set new offset to location of next instruction, ignoring skipping */
				/* We shall make sure the address wraps though */
				for (tmp=0;tmp<b->dims;tmp++)
				{
					ip->origin[tmp] = ip->pos[tmp] + ip->delta[tmp];
					if (b->sizes[tmp] != 0)
						if ((unsigned int) ip->origin[tmp] >= b->sizes[tmp])
						{
							ip->origin[tmp] = ip->origin[tmp] % ((int) b->sizes[tmp]);
							if (ip->origin[tmp] < 0)
								ip->origin[tmp] += b->sizes[tmp];
						}
				}
			}
			else
				bef_unimplemented(b,ip,cell);
			if (ip->flags & IP_SWITCHMODE)
				bef_setspace(b,ip->pos,(int *) NULL,'}');
			break;
		case '|': /* North-south if */
			if (b->dims > 1)
			{
				if ((ip->flags & IP_HOVERMODE) == 0)
					for (tmp=0;tmp<b->dims;tmp++)
						ip->delta[tmp] = 0;
				if (bef_pop(ip) == 0)
					ip->delta[1]++;
				else
					ip->delta[1]--;
				bef_flycheck(b,ip);
			}
			else
				bef_unimplemented(b,ip,cell);
			break;
		case '}': /* End block (98) */
			if ((b->flags & PROG_98) && ((b->flags & PROG_MINIFUNGE) == 0))
			{
				if (bef_stackstackempty(ip))
					bef_reflectip(b,ip);
				else
				{
					tmp = bef_pop(ip);
					/* Get storage offset from SOSS */
					for (tmp2=b->dims-1;tmp2>=0;tmp2--)
						ip->origin[tmp2] = bef_underpop(ip);
					if (tmp > 0)
					{
						tstck = malloc(sizeof(signed long)*tmp); /* Just use a temp array for now */
						for (tmp2=0;tmp2<tmp;tmp2++)
							tstck[tmp2] = bef_pop(ip);
						/* Now write back... */
						while (tmp > 0)
							bef_underpush(ip,tstck[--tmp]);
						free(tstck);
					}
					bef_popstack(ip);
					while (tmp < 0)
					{
						tmp++;
						bef_pop(ip); /* Pop cells off original SOSS */
					}
				}
			}
			else
				bef_unimplemented(b,ip,cell);
			if (ip->flags & IP_SWITCHMODE)
				bef_setspace(b,ip->pos,(int *) NULL,'{');
			break;
		case '~': /* Input character */
			tmp = bef_getchar(b,ip,&tmp2);
			if (tmp2)
				return EXEC_WAIT;
			if (tmp == EOF)
				bef_reflectip(b,ip); /* Error */
			else
				bef_push(ip,tmp);
			break;
		default:
			bef_unimplemented(b,ip,cell);
			break;
	}
	return EXEC_NEXTINSTR; /* Move to next instr. */
}

void bef_nextinstr(bspace *b,bip *ip,int cell)
{
	int tpos[MAX_DIM];
	int tmp;
	/* Move ip to next executable instr. */
	memcpy(tpos,ip->pos,sizeof(tpos));
	if ((cell == ';') && (b->flags & PROG_98) && ((ip->flags & IP_STRINGMODE) == 0))
		tmp = 1; /* jump over mode */
	else
		tmp = 0;
	do {
		if (tmp == 2)
			tmp = 0;
		bef_nextpos(b,ip,1); /* Do big skips */
		cell = bef_readspace(b,ip->pos,(int *) NULL);
		if ((cell == ';') && (b->flags & PROG_98) && ((ip->flags & IP_STRINGMODE) == 0))
			tmp++; /* Toggle mode; 0->1, 1->2. 2 is used as special 'just left' marker */
	} while (((cell == 32) || (tmp != 0)) && (memcmp(tpos,ip->pos,sizeof(tpos)) != 0));
}

void bef_runip(bspace *b,bip *ip)
{
	int cell;
	int code;
	bip *oip;
	oip = ip;
	while (ip->ghost)
	{
		ip = ip->ghost;
		b = ip->space;
	}
	if (ip->flags & IP_PUSHVALUE)
	{
		bef_push(ip,ip->pushvalue);
		ip->flags &= ~IP_PUSHVALUE;
	}
	if (ip->ghostprev == ip->haunted) /* If ip dies, we only want to move oip if it was the one that spawned it (otherwise iterating over dynamic fingerprints/multiple =t's will cause the fingerprint IPs to move) */
		oip = ip->ghostprev;
	else
		oip = (bip *) NULL;
	cell = bef_readspace(b,ip->pos,(int *) NULL);
	if (trace) trace_ip(b,ip,cell);
	if (ip_refresh)
		interface_updatespace(ip->space,ip->pos,0);
	if (ip->flags & IP_STRINGMODE)
	{
		if (cell == '"')
			ip->flags -= IP_STRINGMODE;
		else
		{
			bef_push(ip,cell);
			if (((b->flags & PROG_98) == 0) || (cell != ' ')) /* Move one block */
			{
				bef_nextpos(b,ip,0); /* Don't bigskip */
				if (ip_refresh)
					interface_updatespace(b,ip->pos,0);
				return;
			} /* ... else skip multiple spaces at end of func */
		}
		code = EXEC_NEXTINSTR;
	}
	else
		code = bef_execinstr(b,ip,cell);
	if ((code == EXEC_IMADEMOVE) || (code == EXEC_GHOSTSPAWN))
	{
		if (ip_refresh)
			interface_updatespace(ip->space,ip->pos,0);
		return;
	}
	if (code == EXEC_WAIT)
		return;
	if (code == EXEC_DEAD)
	{
		/* IP has just died */
		/* So move whoever spawned it */
		if (oip)
			ip = oip;
		else
			return; /* No other IPs */
		interface_updatespace(ip->space,ip->pos,0);
	}
	/* Find next instruction */
	if (ip->flags & IP_DEAD)
		return; /* Don't move IP, it's sat over an @ waiting to die */
	bef_nextinstr(ip->space,ip,cell);
	if (ip_refresh)
		interface_updatespace(ip->space,ip->pos,0);
}

void bef_runprog(bspace *b)
{
	bspace *pb;
	bip *c,*n;
	c = b->ips;
	pb = b;
	if (b->flags & PROG_INPUT_PAUSES)
	{
		if (b->input_ip)
			c = b->input_ip; /* Run from input IP onwards */
		else
		{
			/* Scan IP list and find any haunted ones */
			/* There should only ever be one haunted IP if */
			/* input_pauses is used, so this should be OK */
			c = b->ips;
			b = (bspace *) NULL;
			while ((c) && (b == NULL))
			{
				if (c->ghost)
				{
					while (c->ghost)
						c = c->ghost;
					b = c->space;
					break; /* Found one */
				}
				else
					c = c->next;
			}
			if (b == NULL)
			{
				b = pb;
				c = b->ips;
			}
		}
	}
	while ((c != NULL) && (b->ips != NULL))
	{
		n = c->next;
		bef_runip(c->space,c);
		if (((pb->input_ip) || (c->ghost)) && (pb->flags & PROG_INPUT_PAUSES))
			c = (bip *) NULL; /* Someone has input, or a ghost was spawned */
		else
			c = n; /* ... else continue */
	}
}

#endif
