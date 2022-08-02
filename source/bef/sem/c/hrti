#ifndef _SEM_HRTI_C
#define _SEM_HRTI_C

#include <time.h>

#include "hrti.h"

typedef struct _hrti_mark {
	int id;
	int time;
	struct _hrti_mark *next,*prev;
} hrti_mark;

static hrti_mark *hrti_marks = (hrti_mark *) NULL;

static int get_mark(bip *ip)
{
	hrti_mark *m = hrti_marks;
	while (m)
	{
		if (m->id == ip->id)
			return m->time;
		m = m->next;
	}
	return 0; /* Assume a time of 0 won't happen */
}

static void set_mark(bip *ip,int time)
{
	hrti_mark *m = hrti_marks;
	while (m)
	{
		if (m->id == ip->id)
		{
			m->time = time;
			return;
		}
		m = m->next;
	}
	m = malloc(sizeof(hrti_mark));
	m->id = ip->id;
	m->time = time;
	m->next = hrti_marks;
	hrti_marks = m;
	if (m->next)
		m->next->prev = m;
	m->prev = (hrti_mark *) NULL;
}

static void remove_mark(bip *ip)
{
	hrti_mark *m = hrti_marks;
	while (m)
	{
		if (m->id == ip->id)
		{
			if (m->prev)
				m->prev->next = m->next;
			else
				hrti_marks = m->next;
			if (m->next)
				m->next->prev = m->prev;
			free(m);
			return;
		}
		m = m->next;
	}
}

static int hrti_g(bspace *b,bip *ip,int cell)
{
	bef_push(ip,(int) (1000000/CLOCKS_PER_SEC)); /* Trust the C library */
	return EXEC_NEXTINSTR;
}

static int hrti_m(bspace *b,bip *ip,int cell)
{
	set_mark(ip,(int) clock());
	return EXEC_NEXTINSTR;
}

static int hrti_t(bspace *b,bip *ip,int cell)
{
	int t = get_mark(ip);
	if (t == 0)
		bef_reflectip(b,ip);
	else
	{
		t = ((int) clock()) - get_mark(ip); /* Get time in CLOCKS_PER_SEC */
		bef_push(ip,(int) (t*(1000000/CLOCKS_PER_SEC))); /* Get time in microseconds */
	}
	return EXEC_NEXTINSTR;
}

static int hrti_e(bspace *b,bip *ip,int cell)
{
	remove_mark(ip);
	return EXEC_NEXTINSTR;
}

static int hrti_s(bspace *b,bip *ip,int cell)
{
	int t = (int) clock();
	t = t % CLOCKS_PER_SEC; /* Time since the start of the second */
	bef_push(ip,(int) (t*(1000000/CLOCKS_PER_SEC)));
	return EXEC_NEXTINSTR;
}


int install_hrti(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,HRTI_FINGER,'G',hrti_g,load);
	bef_overloadsem(ip,HRTI_FINGER,'M',hrti_m,load);
	bef_overloadsem(ip,HRTI_FINGER,'T',hrti_t,load);
	bef_overloadsem(ip,HRTI_FINGER,'E',hrti_e,load);
	bef_overloadsem(ip,HRTI_FINGER,'S',hrti_s,load);
	return 1;
}

void clone_hrti(bspace *b,bip *to_ip,bip *from_ip)
{
	/* Copy timer mark */
	if (get_mark(from_ip) != 0)
		set_mark(to_ip,get_mark(from_ip));
}

void kill_hrti(bspace *b,bip *ip)
{
	/* Remove any timer mark */
	hrti_e(b,ip,'E');
}


#endif
