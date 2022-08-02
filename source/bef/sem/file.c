#ifndef _SEM_FILE_C
#define _SEM_FILE_C

#include "file.h"

typedef struct _file_mark {
	bspace *b;
	int id;
	int pos[MAX_DIM];
	FILE *f;
	struct _file_mark *next,*prev;
} file_mark;

static file_mark *file_marks = (file_mark *) NULL;
static int max_file = 0;

static file_mark *get_file(bspace *b,int id)
{
	file_mark *m = file_marks;
	while (m)
	{
		if ((m->b == b) && (m->id == id))
			return m;
		m = m->next;
	}
	return 0;
}

static int set_file(bspace *b,FILE *f,int *pos)
{
	int tmp;
	file_mark *m = file_marks;
	while (m)
	{
		if (m->b == b)
			if (f == m->f)
			{
				for (tmp=0;tmp<b->dims;tmp++)
					m->pos[tmp] = pos[tmp];
				return m->id; /* Already set */
			}
		m = m->next;
	}
	m = malloc(sizeof(file_mark));
	m->b = b;
	m->f = f;
	m->id = max_file++;
	for (tmp=0;tmp<b->dims;tmp++)
		m->pos[tmp] = pos[tmp];
	m->next = file_marks;
	file_marks = m;
	if (m->next)
		m->next->prev = m;
	m->prev = (file_mark *) NULL;
	return m->id;
}

static void remove_files(bspace *b)
{
	file_mark *a,*m = file_marks;
	while (m)
	{
		if (m->b == b)
		{
			if (m->prev)
				m->prev->next = m->next;
			else
				file_marks = m->next;
			if (m->next)
				m->next->prev = m->prev;
			fclose(m->f);
			a = m->next;
			free(m);
			m = a;
		}
		else
			m = m->next;
	}
}

static int remove_file(bspace *b,int id)
{
	file_mark *m = file_marks;
	while (m)
	{
		if ((m->b == b) && (m->id == id))
		{
			if (m->prev)
				m->prev->next = m->next;
			else
				file_marks = m->next;
			if (m->next)
				m->next->prev = m->prev;
			fclose(m->f);
			free(m);
			return 1;
		}
		m = m->next;
	}
	return 0;
}

static int file_c(bspace *b,bip *ip,int cell)
{
	if (remove_file(b,bef_pop(ip)) == 0)
		bef_reflectip(b,ip);
	return EXEC_NEXTINSTR;
}

static int file_g(bspace *b,bip *ip,int cell)
{
	file_mark *f;
	char tstr[1024];
	cell = bef_pop(ip);
	f = get_file(b,cell);
	if (f == NULL)
	{
		bef_reflectip(b,ip);
		return EXEC_NEXTINSTR;
	}
	if (fgets(tstr,1024,f->f) == 0)
	{
		bef_reflectip(b,ip);
		return EXEC_NEXTINSTR;
	}
	bef_push(ip,cell);
	bef_pushstring(ip,tstr);
	bef_push(ip,strlen(tstr));
	return EXEC_NEXTINSTR;
}

static int file_l(bspace *b,bip *ip,int cell)
{
	file_mark *f;
	int tmp;
	cell = bef_pop(ip);
	f = get_file(b,cell);
	if (f == NULL)
		bef_reflectip(b,ip);
	else
	{
		tmp = ftell(f->f);
		if (tmp == -1)
			bef_reflectip(b,ip);
		else
		{
			bef_push(ip,cell);
			bef_push(ip,tmp);
		}
	}
	return EXEC_NEXTINSTR;
}

static int file_o(bspace *b,bip *ip,int cell)
{
	char tstr[256];
	int tmp,mode,tmp2;
	int tpos[MAX_DIM];
	FILE *f;
	tmp = 0;
	do {
		if (tmp == 256)
			tmp = 255;
		tstr[tmp++] = bef_pop(ip);
	} while (tstr[tmp-1]);
	mode = bef_pop(ip);
	for (tmp2=b->dims-1;tmp2>=0;tmp2--)
		tpos[tmp2] = bef_pop(ip);
	if (tmp == 256)
	{
		bef_reflectip(b,ip);
		return EXEC_NEXTINSTR;
	}
	f = (FILE *) NULL;
	switch (mode) {
		case 0: f = fopen(tstr,"r"); break;
		case 1: f = fopen(tstr,"w"); break;
		case 2: f = fopen(tstr,"a"); break;
		case 3: f = fopen(tstr,"r+"); break;
		case 4: f = fopen(tstr,"w+"); break;
		case 5: f = fopen(tstr,"a+"); break;
	}
	if (f == NULL)
	{
		bef_reflectip(b,ip);
		return EXEC_NEXTINSTR;
	}
	bef_push(ip,set_file(b,f,tpos));
	return EXEC_NEXTINSTR;
}

static int file_p(bspace *b,bip *ip,int cell)
{
	char tstr[256];
	file_mark *f;
	int tmp;
	tmp = 0;
	do {
		if (tmp == 256)
			tmp = 255;
		tstr[tmp++] = bef_pop(ip);
	} while (tstr[tmp-1]);
	cell = bef_pop(ip);
	f = get_file(b,cell);
	if ((tmp == 256) || (f == NULL))
		bef_reflectip(b,ip);
	else if (fputs(tstr,f->f) == EOF)
		bef_reflectip(b,ip);
	else
		bef_push(ip,cell);
	return EXEC_NEXTINSTR;
}

static int file_r(bspace *b,bip *ip,int cell)
{
	int tpos[MAX_DIM];
	file_mark *f;
	int tmp,tmp2;
	for (tmp=0;tmp<b->dims;tmp++)
		tpos[tmp] = 0;
	tmp = bef_pop(ip);
	cell = bef_pop(ip);
	f = get_file(b,cell);
	if (f == NULL)
	{
		bef_reflectip(b,ip);
		return EXEC_NEXTINSTR;
	}
	while (tmp > 0)
	{
		tmp2 = fgetc(f->f);
		if (tmp2 == EOF)
		{
			bef_reflectip(b,ip);
			return EXEC_NEXTINSTR;
		}
		bef_setspace(b,f->pos,tpos,tmp2);
		tpos[0]++;
		tmp--;
	}
	bef_push(ip,cell);
	return EXEC_NEXTINSTR;
}

static int file_s(bspace *b,bip *ip,int cell)
{
	file_mark *f;
	int m,n;
	n = bef_pop(ip);
	m = bef_pop(ip);
	cell = bef_pop(ip);
	f = get_file(b,cell);
	if ((f == NULL) || (m < 0) || (m > 2))
	{
		bef_reflectip(b,ip);
		return EXEC_NEXTINSTR;
	}
	if (m == 0)
		m = SEEK_SET;
	else if (m == 1)
		m = SEEK_CUR;
	else if (m == 2)
		m = SEEK_END;
	if (fseek(f->f,n,m))
		bef_reflectip(b,ip);
	else
		bef_push(ip,cell);
	return EXEC_NEXTINSTR;
}

static int file_w(bspace *b,bip *ip,int cell)
{
	int tpos[MAX_DIM];
	file_mark *f;
	int tmp,tmp2;
	for (tmp=0;tmp<b->dims;tmp++)
		tpos[tmp] = 0;
	tmp = bef_pop(ip);
	cell = bef_pop(ip);
	f = get_file(b,cell);
	if (f == NULL)
	{
		bef_reflectip(b,ip);
		return EXEC_NEXTINSTR;
	}
	while (tmp > 0)
	{
		tmp2 = fputc(bef_readspace(b,f->pos,tpos) & 0xFF,f->f);
		if (tmp2 == EOF)
		{
			bef_reflectip(b,ip);
			return EXEC_NEXTINSTR;
		}
		tpos[0]++;
		tmp--;
	}
	bef_push(ip,cell);
	return EXEC_NEXTINSTR;
}


int install_file(bspace *b,bip *ip,int load)
{
	if ((b->flags & PROG_FS) == 0)
		return 0;
	bef_overloadsem(ip,FILE_FINGER,'C',file_c,load);
	bef_overloadsem(ip,FILE_FINGER,'G',file_g,load);
	bef_overloadsem(ip,FILE_FINGER,'L',file_l,load);
	bef_overloadsem(ip,FILE_FINGER,'O',file_o,load);
	bef_overloadsem(ip,FILE_FINGER,'P',file_p,load);
	bef_overloadsem(ip,FILE_FINGER,'R',file_r,load);
	bef_overloadsem(ip,FILE_FINGER,'S',file_s,load);
	bef_overloadsem(ip,FILE_FINGER,'W',file_w,load);
	return 1;
}

void killprog_file(bspace *b)
{
	remove_files(b);
}


#endif
