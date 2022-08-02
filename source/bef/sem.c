#ifndef _BEF_SEM_C
#define _BEF_SEM_C

#include "sem.h"
#include "main.h"
#include "stack.h"
#include "dynafing.h"

#include "sem/null.h"
#include "sem/roma.h"
#include "sem/hrti.h"
#include "sem/refc.h"
#include "sem/modu.h"
#include "sem/orth.h"
#include "sem/toys.h"
#include "sem/mode.h"
#include "sem/turt.h"
#include "sem/perl.h"
#include "sem/base.h"
#include "sem/cpli.h"
#include "sem/dirf.h"
#include "sem/file.h"
#include "sem/frth.h"
#include "sem/term.h"
#include "sem/fixp.h"

typedef struct {
	int fingerprint;
	int (*load)(bspace *b,bip *ip,int load); /* For loading/unloading. Return 1 if success, 0 if fail (i.e. fingerprint been disabled) */
	void (*clone)(bspace *b,bip *to_ip,bip *from_ip); /* For cloning an IP; semantic stack already copied. Called for all semantics */
	void (*kill)(bspace *b,bip *ip); /* For death of IP. Called for all semantics */
	void (*killprog)(bspace *b); /* For closure of program. Called for all semantics */
} _sem;

int num_internal_semantics = 17;
_sem internal_semantics[17] = {
	{NULL_FINGER,install_null,0,0,0},
	{ROMA_FINGER,install_roma,0,0,0},
	{HRTI_FINGER,install_hrti,clone_hrti,kill_hrti,0},
	{MODU_FINGER,install_modu,0,0,0},
	{ORTH_FINGER,install_orth,0,0,0},
	{REFC_FINGER,install_refc,0,0,killprog_refc},
	{TOYS_FINGER,install_toys,0,0,0},
	{MODE_FINGER,install_mode,0,0,0},
	{TURT_FINGER,install_turt,0,0,killprog_turt},
	{PERL_FINGER,install_perl,0,0,0},
	{BASE_FINGER,install_base,0,0,0},
	{CPLI_FINGER,install_cpli,0,0,0},
	{DIRF_FINGER,install_dirf,0,0,0},
	{FILE_FINGER,install_file,0,0,killprog_file},
	{FRTH_FINGER,install_frth,0,0,0},
	{TERM_FINGER,install_term,0,0,0},
	{FIXP_FINGER,install_fixp,0,0,0}};

void bef_loadsem(bspace *b,bip *ip)
{
	int c,fing;
	c = bef_pop(ip);
	fing = 0;
	if (c > bef_stacksize(ip,0)+4) /* Protect against nasty bugs */
	{
		c = bef_stacksize(ip,0);
		while (c--)
			bef_pop(ip);
	}
	else
		while (c--)
			fing = (fing*256) + bef_pop(ip);
	if (fing == 0)
	{
		bef_reflectip(b,ip); /* Doh! can't support null fingerprints since that's the code for the invisible one */
		return;
	}
	/* Try and find semantics for it... */
	if (install_dynafing(b,ip,fing))
		return; /* Found dynamic fingerprint */
	for (c=0;c<num_internal_semantics;c++)
		if (internal_semantics[c].fingerprint == fing)
		{
			if ((internal_semantics[c].load)(b,ip,1))
			{
				bef_push(ip,fing);
				bef_push(ip,1);
				return;
			}
		}
	/* Else fail */
	bef_reflectip(b,ip);
}

void bef_unloadsem(bspace *b,bip *ip)
{
	int c,fing;
	c = bef_pop(ip);
	fing = 0;
	if (c > bef_stacksize(ip,0)+4) /* Protect against nasty bugs */
	{
		c = bef_stacksize(ip,0);
		while (c--)
			bef_pop(ip);
	}
	else
		while (c--)
			fing = (fing*256) + bef_pop(ip);
	if (fing == 0)
	{
		bef_reflectip(b,ip);
		return;
	}
	if (uninstall_dynafing(b,ip,fing))
		return;
	for (c=0;c<num_internal_semantics;c++)
		if (internal_semantics[c].fingerprint == fing)
			if ((internal_semantics[c].load)(b,ip,0))
				return;
	bef_reflectip(b,ip); /* Semantics not recognised */
}

void bef_overloadsem(bip *ip,int fing,int cell,int (*instr)(bspace *b,bip *ip,int cell),int load)
{
	bsemantics *s;
	if (load)
	{
		s = malloc(sizeof(bsemantics));
		s->instr = instr;
		s->fing = fing;
		s->next = ip->semantics[cell-'A'];
		ip->semantics[cell-'A'] = s;
	}
	else if ((ip->semantics[cell-'A']) && ((fing == 0) || (ip->semantics[cell-'A']->fing != 0))) /* Protect invisible fingerprint */
	{
		s = ip->semantics[cell-'A'];
		ip->semantics[cell-'A'] = s->next;
		free(s);
	}
}

int bef_callsem(bspace *b,bip *ip,int cell)
{
	if (ip->semantics[cell-'A'])
	{
		return (ip->semantics[cell-'A']->instr)(b,ip,cell);
	}
	/* else unimplemented */
	bef_unimplemented(b,ip,cell);
	return EXEC_NEXTINSTR;
}

void bef_initsem(bip *ip)
{
	int t;
	for (t=0;t<26;t++)
		ip->semantics[t] = 0;
}

void bef_clonesem(bspace *b,bip *ip,bip *clone)
{
	/* Copy installed semantics from clone to ip */
	bsemantics *old,*clones,*n;
	int tmp;
	for (tmp=0;tmp<26;tmp++)
	{
		old = ip->semantics[tmp] = (bsemantics *) NULL;
		clones = clone->semantics[tmp];
		while (clones)
		{
			n = malloc(sizeof(bsemantics)); /* Create new one */
			n->next = (bsemantics *) NULL;
			if (old == NULL) /* Link old one up to it */
				ip->semantics[tmp] = n;
			else
				old->next = n;
			n->instr = clones->instr;
			old = n; /* Move on to next frame */
			clones = clones->next;
		}
	}
	dynafing_clone(b,ip,clone);
	for (tmp=0;tmp<num_internal_semantics;tmp++)
		if (internal_semantics[tmp].clone != 0)
			(internal_semantics[tmp].clone)(b,ip,clone);
}

void bef_killsem(bspace *b,bip *ip)
{
	/* Call the kill code for all semantics & delete semantics assigned to IP */
	int c;
/*	dynafing_kill(b,ip); - Now handled by execinstr to ensure that IPs can be killed immediately via killip */
	for (c=0;c<num_internal_semantics;c++)
		if (internal_semantics[c].kill != 0)
			(internal_semantics[c].kill)(b,ip);
	for (c=0;c<26;c++)
		while (ip->semantics[c])
			bef_overloadsem(ip,0,c+'A',0,0); /* Use load/unload code */
}

void bef_killprogsem(bspace *b)
{
	/* Call the kill prog code for all semantics */
	int c;
	dynafing_killprog(b);
	for (c=0;c<num_internal_semantics;c++)
		if (internal_semantics[c].killprog != 0)
			(internal_semantics[c].killprog)(b);
}

#endif
