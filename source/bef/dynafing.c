#ifndef _BEF_DYNAFING_C
#define _BEF_DYNAFING_C

#include "dynafing.h"
#include "main.h"
#include "stack.h"
#include "mem.h"
#include "sem.h"
#include "prog.h"
#include "ip.h"

#ifndef LYT
#define DYNAFINGDIR "<Befunge$DynaFing>."
#else
#include "../lyt.h"
#endif

static void do_dyna_install(bspace *b,bip *ip,bspace *bs,int load)
{
	int t,tpos[MAX_DIM];
	for (t=0;t<bs->dims;t++)
		tpos[t] = 0;
	bs->flags &= ~(PROG_DYNA_CON + PROG_DYNA_KILL);
	while (tpos[1] <= bs->maxmem[1])
	{
		if (bef_readspace(bs,tpos,(int *) 0) == '=')
		{
			tpos[0] = 1;
			t = bef_readspace(bs,tpos,(int *) 0);
			if ((t >= 'A') && (t <= 'Z'))
				bef_overloadsem(ip,bs->fing,t,dynafing_do,load);
			else if ((t == 't') && (load))
				bs->flags |= PROG_DYNA_CON;
			else if ((t == '@') && (load))
				bs->flags |= PROG_DYNA_KILL;
			tpos[0] = 0;
		}
		tpos[1]++;
	}
	if (load)
	{
		bef_push(ip,bs->fing);
		bef_push(ip,1);
	}
}

int install_dynafing(bspace *b,bip *ip,int fing)
{
	/* Search dynamic fingerprints for 'fing' */
	/* Return 1 if found */
	/* First check loaded ones... */
	bspace *bs;
	char tstr[64];
	FILE *f;
	int t,tpos[MAX_DIM];
	int mini;
	bs = progs;
	mini = 0;
	while (bs)
	{
		if (bs->flags & PROG_FINGERPRINT)
			if ((bs->fing == fing) && (bs->haunted == b))
			{
				/* Found one */
				/* So install it */
				do_dyna_install(b,ip,bs,1);
				return 1;
			}
		bs = bs->next;
	}
	/* ... else none found */
	/* So try <Befunge$DynaFing>.<hex> */
	/* or <Befunge$DynaFing>.<ascii> */
	sprintf(tstr,DYNAFINGDIR "0x%08x",fing);
	f = fopen(tstr,"r");
	if (f == NULL)
	{
		t = (fing >> 24) & 0xFF;
		if ((t < 33) || (t == 127))
			return 0;
		tstr[19] = t;
		t = (fing >> 16) & 0xFF;
		if ((t < 33) || (t == 127))
			return 0;
		tstr[20] = t;
		t = (fing >> 8) & 0xFF;
		if ((t < 33) || (t == 127))
			return 0;
		tstr[21] = t;
		t = fing & 0xFF;
		if ((t < 33) || (t == 127))
			return 0;
		tstr[22] = t;
		tstr[23] = 0;
		f = fopen(tstr,"r");
		if (f == NULL)
		{
			/* Try minifunge with /fl extension */
#ifndef LYT
			strcat(tstr,"/fl");
#else
			strcat(tstr,".fl");
#endif
			f = fopen(tstr,"r");
			if (f)
				mini = 1;
		}
	}
	if (f == NULL)
		return 0;
	/* .. else load */
	fclose(f);
	tpos[0] = tpos[1] = 0; /* Unlimited X,Y */
	for (t=2;t<MAX_DIM;t++)
		tpos[t] = 1; /* No others */
	bs = bef_newprog(2,tpos,32); /* Use 32bit fungespace */
	strcpy(bs->fname,tstr);
	if (load_file(bs,tstr,(int *) 0,0,(int *) 0) == EOF)
	{
		bef_killprog(bs);
		return 0; /* Can't load! */
	}
	/* Now copy enviornment... */
	bs->flags = PROG_98 + PROG_FINGERPRINT + (b->flags & (PROG_FS + PROG_NOPROMPT + PROG_WARN_UNIMPLEMENTED + PROG_INPUT_PAUSES));
	if (mini)
	{
		bs->flags |= PROG_MINIFUNGE;
		bs->flags &= ~PROG_FS; /* Stop =,i,o */
	}
	/* All others are unset */
	bs->haunted = b;
	bs->fing = fing;
	do_dyna_install(b,ip,bs,1);
	return 1;
}

int uninstall_dynafing(bspace *b,bip *ip,int fing)
{
	/* Search for installed dynafing... */
	int t,tpos[MAX_DIM];
	bspace *bs;
	char tstr[64];
	FILE *f;
	bs = progs;
	while (bs)
	{
		if (bs->flags & PROG_FINGERPRINT)
			if ((bs->fing == fing) && (bs->haunted == b))
			{
				do_dyna_install(b,ip,bs,0);
				return 1;
			}
		bs = bs->next;
	}
	/* ... else might be an unloaded one */
	sprintf(tstr,DYNAFINGDIR "0x%08x",fing);
	f = fopen(tstr,"r");
	if (f == NULL)
	{
		t = (fing >> 24) & 0xFF;
		if ((t < 33) || (t == 127))
			return 0;
		tstr[19] = t;
		t = (fing >> 16) & 0xFF;
		if ((t < 33) || (t == 127))
			return 0;
		tstr[20] = t;
		t = (fing >> 8) & 0xFF;
		if ((t < 33) || (t == 127))
			return 0;
		tstr[21] = t;
		t = fing & 0xFF;
		if ((t < 33) || (t == 127))
			return 0;
		tstr[22] = t;
		tstr[23] = 0;
		f = fopen(tstr,"r");
		if (f == NULL)
		{
			/* Try minifunge with /fl extension */
#ifndef LYT
			strcat(tstr,"/fl");
#else
			strcat(tstr,".fl");
#endif
			f = fopen(tstr,"r");
		}
	}
	if (f == NULL)
		return 0;
	/* .. else load */
	fclose(f);
	tpos[0] = tpos[1] = 0; /* Unlimited X,Y */
	for (t=2;t<MAX_DIM;t++)
		tpos[t] = 1; /* No others */
	bs = bef_newprog(2,tpos,8); /* Use 8bit fungespace since it's only temporarily loaded */
	strcpy(bs->fname,tstr);
	if (load_file(bs,tstr,(int *) 0,0,(int *) 0) == EOF)
	{
		bef_killprog(bs);
		return 0; /* Can't load! */
	}
	do_dyna_install(b,ip,bs,0);
	bef_killprog(bs);
	return 1;
}

void dynafing_clone(bspace *b,bip *ip,bip *clone)
{
	/* IP is the copy of clone */
	/* Not very logical naming */
	bspace *bs;
	bs = progs;
	while (bs)
	{
		if (bs->flags & PROG_FINGERPRINT)
			if ((bs->haunted == b) && (bs->flags & PROG_DYNA_CON))
			{
				/* Found one */
				dynafing_do_do(b,ip,bs,'t');
				ip->ghost->pushvalue = clone->id;
				ip->ghost->flags |= IP_PUSHVALUE;
			}
		bs = bs->next;
	}
}

void dynafing_kill(bspace *b,bip *ip)
{
	bspace *bs;
	bs = progs;
	while (bs)
	{
		if (bs->flags & PROG_FINGERPRINT)
			if ((bs->haunted == b) && (bs->flags & PROG_DYNA_KILL))
				dynafing_do_do(b,ip,bs,'@');
		bs = bs->next;
	}
}

void dynafing_killprog(bspace *b)
{
	/* Remove any fingerprint bspaces... */
	bspace *bs,*n;
	bs = progs;
	while (bs)
	{
		n = bs->next;
		if (bs->flags & PROG_FINGERPRINT)
			if (bs->haunted == b)
			{
				bef_killprog(bs);
				n = progs; /* Might cause cascade delete, which would be bad */
			}
		bs = n;
	}
}

int dynafing_do(bspace *b,bip *ip,int cell)
{
	int fing;
	bspace *bs;
	/* Find what fingerprint we are */
	fing = ip->semantics[cell-'A']->fing;
	/* Now find fingerprint space for it */
	bs = progs;
	while (bs)
	{
		if (bs->flags & PROG_FINGERPRINT)
			if ((bs->haunted == b) && (bs->fing == fing))
				return dynafing_do_do(b,ip,bs,cell);
		bs = bs->next;
	}
	/* ... else not found. shouldn't happen. */
	bef_unimplemented(b,ip,cell);
	return EXEC_NEXTINSTR;
}

void dynafing_stacktransplant(bip *a,bip *b)
{
	/* Swap stacks */
	bstackstack *s;
	s = a->stack;
	a->stack = b->stack;
	b->stack = s;
}

static int dynafing_b(bspace *b,bip *ip,int cell)
{
	bef_reflectip(b->haunted,ip->haunted);
	bef_nextpos(b->haunted,ip->haunted,0);
	bef_reflectip(b->haunted,ip->haunted);
	return EXEC_NEXTINSTR;
}

static int dynafing_d(bspace *b,bip *ip,int cell)
{
	int t;
	for (t=b->haunted->dims-1;t>=0;t--)
		ip->haunted->delta[t] = bef_pop(ip);
	bef_flycheck(b->haunted,ip->haunted);
	return EXEC_NEXTINSTR;
}

static int dynafing_e(bspace *b,bip *ip,int cell)
{
	bef_push(ip,bef_stacksize(ip,0));
	return EXEC_NEXTINSTR;
}

static int dynafing_f(bspace *b,bip *ip,int cell)
{
	bef_nextpos(b->haunted,ip->haunted,0);
	return EXEC_NEXTINSTR;
}

static int dynafing_g(bspace *b,bip *ip,int cell)
{
	int t,tpos[MAX_DIM];
	for (t=b->haunted->dims-1;t>=0;t--)
		tpos[t] = bef_pop(ip);
	if (b->flags & PROG_MINIFUNGE)
		bef_push(ip,bef_readspace(b,tpos,ip->origin));
	else
		bef_push(ip,bef_readspace(b->haunted,tpos,ip->haunted->origin));
	return EXEC_NEXTINSTR;
}

static int dynafing_k(bspace *b,bip *ip,int cell)
{
	/* Pick stack */
	int n;
	n = bef_pop(ip);
	if (n < 0)
		bef_reflectip(b,ip);
	else
		bef_push(ip,bef_peekstack(ip,n));
	return EXEC_NEXTINSTR;
}

static int dynafing_l(bspace *b,bip *ip,int cell)
{
	int t;
	for (t=b->haunted->dims-1;t>=0;t--)
		ip->haunted->pos[t] = bef_pop(ip) - ip->haunted->delta[t];
	return EXEC_NEXTINSTR;
}

static int dynafing_m(bspace *b,bip *ip,int cell)
{
	cell = 0;
	if (ip->haunted->flags & IP_STRINGMODE)
		cell |= 1;
	if (ip->haunted->flags & IP_HOVERMODE)
		cell |= 2;
	if (ip->haunted->flags & IP_INVERTMODE)
		cell |= 4;
	if (ip->haunted->flags & IP_QUEUEMODE)
		cell |= 8;
	if (ip->haunted->flags & IP_SWITCHMODE)
		cell |= 16;
	bef_push(ip,cell);
	return EXEC_NEXTINSTR;
}

static int dynafing_o(bspace *b,bip *ip,int cell)
{
	int n,t;
	int *z;
	n = bef_pop(ip);
	if (n >= bef_stacksize(ip,0))
		bef_push(ip,0);
	else if (n < 0)
		bef_reflectip(b,ip);
	else if (n > 0)
	{
		/* We need the roll part to work according to queuemode */
		/* So if queuemode is on, temporarily turn invertmode on */
		cell = ip->flags & IP_INVERTMODE;
		if (ip->flags & IP_QUEUEMODE)
			ip->flags |= IP_INVERTMODE;
		else
			ip->flags &= ~IP_INVERTMODE;
		z = malloc(sizeof(int)*n);
		for (t=0;t<n;t++)
			z[t] = bef_pop(ip);
		for (t=n-2;t>=0;t--)
			bef_push(ip,z[t]);
		/* Restore invertmode */
		ip->flags = (ip->flags & ~IP_INVERTMODE) | cell;
		bef_push(ip,z[n-1]);
		free(z);
	}
	return EXEC_NEXTINSTR;
}

static int dynafing_p(bspace *b,bip *ip,int cell)
{
	int t,tpos[MAX_DIM];
	for (t=b->haunted->dims-1;t>=0;t--)
		tpos[t] = bef_pop(ip);
	if (b->flags & PROG_MINIFUNGE)
		bef_setspace(b,tpos,ip->origin,bef_pop(ip));
	else
		bef_setspace(b->haunted,tpos,ip->haunted->origin,bef_pop(ip));
	return EXEC_NEXTINSTR;
}

static int dynafing_r(bspace *b,bip *ip,int cell)
{
	bef_reflectip(b->haunted,ip->haunted);
	return EXEC_NEXTINSTR;
}

static int dynafing_s(bspace *b,bip *ip,int cell)
{
	int t;
	for (t=b->haunted->dims-1;t>=0;t--)
		ip->haunted->origin[t] = bef_pop(ip);
	return EXEC_NEXTINSTR;
}

static int dynafing_y(bspace *b,bip *ip,int cell)
{
	cell = ip->haunted->flags & (IP_INVERTMODE | IP_QUEUEMODE);
	ip->haunted->flags = (ip->haunted->flags & ~(IP_INVERTMODE | IP_QUEUEMODE)) | (ip->flags & (IP_INVERTMODE | IP_QUEUEMODE)); /* Impose our invertmode/queuemode flags */
	dynafing_stacktransplant(ip,ip->haunted);
	bef_y(b->haunted,ip->haunted);
	dynafing_stacktransplant(ip,ip->haunted);
	ip->haunted->flags = (ip->haunted->flags & ~(IP_INVERTMODE | IP_QUEUEMODE)) | cell;
	return EXEC_NEXTINSTR;
}

int dynafing_do_do(bspace *b,bip *ip,bspace *bs,int cell)
{
	/* Execute cell from bs */
	bip *i;
	int t;
	/* Assume it exists */
	i = bef_newip(bs,(bip *) NULL);
	/* Locate start position */
	t = 0;
	while (t == 0)
	{
		if (bef_readspace(bs,i->pos,(int *) 0) == '=')
		{
			i->pos[0]++;
			if (bef_readspace(bs,i->pos,(int *) 0) == cell)
				t = 1;
			i->pos[0]--;
		}
		i->pos[1]++;
		if (i->pos[1] > bs->maxmem[1])
		{
			/* Not found */
			bef_killip(bs,i);
			if ((cell != 't') && (cell != '@'))
				bef_reflectip(b,ip);
			return EXEC_NEXTINSTR;
		}
	}
	if (b->flags & PROG_MINIFUNGE)
		for (t=0;t<b->dims;t++)
			i->origin[t] = i->pos[t]; /* emulate the isolation of mini funge programs */
	i->delta[0] = 1;
	dynafing_stacktransplant(ip,i);
	i->haunted = ip;
	i->ghost = ip->ghost; /* Insert ourselves into ghost list just above current IP */
	ip->ghost = i;
	if (i->ghost)
		i->ghost->ghostprev = i;
	i->ghostprev = ip;
	/* Install the invisible fingerprint */
	bef_overloadsem(i,0,'B',dynafing_b,1);
	bef_overloadsem(i,0,'D',dynafing_d,1);
	bef_overloadsem(i,0,'E',dynafing_e,1);
	bef_overloadsem(i,0,'F',dynafing_f,1);
	bef_overloadsem(i,0,'G',dynafing_g,1);
	bef_overloadsem(i,0,'K',dynafing_k,1);
	bef_overloadsem(i,0,'L',dynafing_l,1);
	if ((b->flags & PROG_MINIFUNGE) == 0)
		bef_overloadsem(i,0,'M',dynafing_m,1);
	bef_overloadsem(i,0,'O',dynafing_o,1);
	bef_overloadsem(i,0,'P',dynafing_p,1);
	bef_overloadsem(i,0,'R',dynafing_r,1);
	if ((b->flags & PROG_MINIFUNGE) == 0)
	{
		bef_overloadsem(i,0,'S',dynafing_s,1);
		bef_overloadsem(i,0,'Y',dynafing_y,1);
	}
	if (nowimp) /* Can run dynamic fingerprints quickly if single tasking */
	{
		while (bs->ips != NULL)
			bef_runprog(bs);
		return EXEC_IMADEMOVE; /* runip will have moved us already */
	}
	return EXEC_GHOSTSPAWN;
}

#endif
