#ifndef _SEM_REFC_C
#define _SEM_REFC_C

#include "refc.h"

typedef struct _refc_mark {
	bspace *b;
	int id;
	int pos[MAX_DIM];
	struct _refc_mark *next,*prev;
} refc_mark;

static refc_mark *refc_marks = (refc_mark *) NULL;
static int max_refc = 0;

static int *get_mark(bspace *b,int id)
{
	refc_mark *m = refc_marks;
	while (m)
	{
		if ((m->b == b) && (m->id == id))
			return m->pos;
		m = m->next;
	}
	return 0;
}

static int set_mark(bspace *b,int *pos)
{
	int t;
	refc_mark *m = refc_marks;
	while (m)
	{
		if (m->b == b)
		{
			for (t=0;t<b->dims;t++)
				if (m->pos[t] != pos[t])
					t = b->dims+10;
			if (t == b->dims)
				return m->id; /* Already set */
		}
		m = m->next;
	}
	m = malloc(sizeof(refc_mark));
	m->b = b;
	for (t=0;t<b->dims;t++)
		m->pos[t] = pos[t];
	m->id = max_refc++; /* Could cause problems with 8 bit cells... */
	m->next = refc_marks;
	refc_marks = m;
	if (m->next)
		m->next->prev = m;
	m->prev = (refc_mark *) NULL;
	return m->id;
}

static void remove_marks(bspace *b)
{
	refc_mark *a,*m = refc_marks;
	while (m)
	{
		if (m->b == b)
		{
			if (m->prev)
				m->prev->next = m->next;
			else
				refc_marks = m->next;
			if (m->next)
				m->next->prev = m->prev;
			a = m->next;
			free(m);
			m = a;
		}
		else
			m = m->next;
	}
}

static int refc_r(bspace *b,bip *ip,int cell)
{
	int pos[MAX_DIM],t;
	for (t=b->dims-1;t>=0;t--)
		pos[t] = bef_pop(ip);
	bef_push(ip,set_mark(b,pos));
	return EXEC_NEXTINSTR;
}

static int refc_d(bspace *b,bip *ip,int cell)
{
	int *pos,t;
	pos = get_mark(b,bef_pop(ip));
	if (pos)
		for (t=0;t<b->dims;t++)
			bef_push(ip,pos[t]);
	else
		bef_reflectip(b,ip);
	return EXEC_NEXTINSTR;
}


int install_refc(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,REFC_FINGER,'R',refc_r,load);
	bef_overloadsem(ip,REFC_FINGER,'D',refc_d,load);
	return 1;
}

void killprog_refc(bspace *b)
{
	remove_marks(b);
}


#endif
