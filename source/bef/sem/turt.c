#ifndef _SEM_TURT_C
#define _SEM_TURT_C

#include <math.h>
#ifndef LYT
#include "kernel.h"
#include "swis.h"
#else
#include "../../lyt.h"
#endif

#include "turt.h"

#define RADPERDEG 0.017453293

#define TURT_SCALE 256

typedef struct _line {
	int sx,sy,ex,ey;
	int col;
	struct _line *next;
} line;

typedef struct _turt {
	bspace *b;
	struct _turt *next,*prev;
	int x,y,ang,pencol,pendown,papercol,display;
	int minx,miny,maxx,maxy;
	line *lines,*last;
} turt;

static turt *turtles = (turt *) NULL;

static turt *add_turtle(bspace *b)
{
	turt *t;
	t = turtles;
	while (t)
	{
		if (t->b == b)
			return t;
		t = t->next;
	}
	t = malloc(sizeof(turt));
	t->next = turtles;
	turtles = t;
	t->prev = (turt *) NULL;
	if (t->next)
		t->next->prev = t;
	t->b = b;
	t->x = t->y = t->ang = t->pencol = t->pendown = t->display = 0;
	t->papercol = 0xFFFFFF; /* White */
	t->minx = t->miny = t->maxx = t->maxy = 0;
	t->lines = t->last = (line *) NULL;
	return t;
}

static void remove_turtle(bspace *b)
{
	turt *t;
	line *l;
	t = turtles;
	while (t)
	{
		if (t->b == b)
		{
			if (t->next)
				t->next->prev = t->prev;
			if (t->prev)
				t->prev->next = t->next;
			else
				turtles = t->next;
			while (t->lines)
			{
				l = t->lines;
				t->lines = l->next;
				free(l);
			}
			free(t);
			return;
		}
		t = t->next;
	}
}

static turt *find_turtle(bspace *b)
{
	turt *t;
	t = turtles;
	while (t)
	{
		if (t->b == b)
			return t;
	}
	return add_turtle(b); /* Shouldn't happen! */
}

static int turt_l(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	t->ang += bef_pop(ip);
	t->ang = t->ang % 360;
	if (t->ang < 0)
		t->ang += 360;
	return EXEC_NEXTINSTR;
}

static int turt_r(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	t->ang -= bef_pop(ip);
	t->ang = t->ang % 360;
	if (t->ang < 0)
		t->ang += 360;
	return EXEC_NEXTINSTR;
}

static int turt_h(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	t->ang = bef_pop(ip);
	t->ang = t->ang % 360;
	if (t->ang < 0)
		t->ang += 360;
	return EXEC_NEXTINSTR;
}

static int turt_f(bspace *b,bip *ip,int cell)
{
	int dist,nx,ny;
	turt *t;
	line *l;
	t = find_turtle(b);
	dist = bef_pop(ip);
	if (dist)
	{
		nx = t->x + ((int) (cos(t->ang*RADPERDEG)*dist*TURT_SCALE));
		ny = t->y + ((int) (sin(t->ang*RADPERDEG)*dist*TURT_SCALE));
	}
	else
	{
		nx = t->x;
		ny = t->y+1;
	}
	if (t->pendown)
	{
		l = malloc(sizeof(line));
		l->next = (line *) NULL;
		if (t->last)
			t->last->next = l;
		else
			t->lines = l;
		t->last = l;
		l->sx = t->x;
		l->sy = t->y;
		l->ex = nx;
		l->ey = ny;
		l->col = t->pencol;
	}
	t->x = nx;
	t->y = ny;
	if (t->minx > nx)
		t->minx = nx;
	else if (t->maxx < nx)
		t->maxx = nx;
	if (t->miny > ny)
		t->miny = ny;
	else if (t->maxy < ny)
		t->maxy = ny;
	return EXEC_NEXTINSTR;
}

static int turt_b(bspace *b,bip *ip,int cell)
{
	int dist,nx,ny;
	turt *t;
	line *l;
	t = find_turtle(b);
	dist = -bef_pop(ip);
	if (dist)
	{
		nx = t->x + ((int) (cos(t->ang*RADPERDEG)*dist*TURT_SCALE));
		ny = t->y + ((int) (sin(t->ang*RADPERDEG)*dist*TURT_SCALE));
	}
	else
	{
		nx = t->x;
		ny = t->y+1;
	}
	if (t->pendown)
	{
		l = malloc(sizeof(line));
		l->next = (line *) NULL;
		if (t->last)
			t->last->next = l;
		else
			t->lines = l;
		t->last = l;
		l->sx = t->x;
		l->sy = t->y;
		l->ex = nx;
		l->ey = ny;
		l->col = t->pencol;
	}
	t->x = nx;
	t->y = ny;
	if (t->minx > nx)
		t->minx = nx;
	else if (t->maxx < nx)
		t->maxx = nx;
	if (t->miny > ny)
		t->miny = ny;
	else if (t->maxy < ny)
		t->maxy = ny;
	return EXEC_NEXTINSTR;
}

static int turt_p(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	cell = bef_pop(ip);
	if (cell == 1)
		t->pendown = 1;
	else if (cell == 0)
		t->pendown = 0;
	else
		bef_reflectip(b,ip);
	return EXEC_NEXTINSTR;
}

static int turt_c(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	t->pencol = bef_pop(ip) & 0xFFFFFF;
	return EXEC_NEXTINSTR;
}

static int turt_n(bspace *b,bip *ip,int cell)
{
	turt *t;
	line *l;
	t = find_turtle(b);
	t->papercol = bef_pop(ip) & 0xFFFFFF;
	while (t->lines)
	{
		l = t->lines;
		t->lines = l->next;
		free(l);
	}
	return EXEC_NEXTINSTR;
}

static int turt_d(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	cell = bef_pop(ip);
	if (cell == 1)
		t->display = 1;
	else if (cell == 0)
		t->display = 0;
	else
		bef_reflectip(b,ip);
	return EXEC_NEXTINSTR;
}

static int turt_t(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	t->y = bef_pop(ip)*TURT_SCALE;
	t->x = bef_pop(ip)*TURT_SCALE;
	return EXEC_NEXTINSTR;
}

static int turt_e(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	bef_push(ip,t->pendown);
	return EXEC_NEXTINSTR;
}

static int turt_a(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	bef_push(ip,t->ang);
	return EXEC_NEXTINSTR;
}

static int turt_q(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	bef_push(ip,t->x/TURT_SCALE);
	bef_push(ip,t->y/TURT_SCALE);
	return EXEC_NEXTINSTR;
}

static int turt_u(bspace *b,bip *ip,int cell)
{
	turt *t;
	t = find_turtle(b);
	bef_push(ip,t->minx/TURT_SCALE); /* Push min then max */
	bef_push(ip,t->miny/TURT_SCALE);
	bef_push(ip,t->maxx/TURT_SCALE);
	bef_push(ip,t->maxy/TURT_SCALE);
	return EXEC_NEXTINSTR;
}

static void fputw(int w,FILE *f)
{
	fputc(w & 0xFF,f);
	fputc((w >> 8) & 0xFF,f);
	fputc((w >> 16) & 0xFF,f);
	fputc((w >> 24) & 0xFF,f);
}

static void fputwbig(int w,FILE *f)
{
	fputc((w >> 24) & 0xFF,f);
	fputc((w >> 16) & 0xFF,f);
	fputc((w >> 8) & 0xFF,f);
	fputc(w & 0xFF,f);
}

static void fputstring(char *s,int len,char pad,FILE *f)
{
	while (len--)
	{
		if (*s)
			fputc(*s++,f);
		else
			fputc(pad,f);
	}
}

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

static int turt_i(bspace *b,bip *ip,int cell)
{
	turt *t;
	line *l;
	FILE *f;
#ifndef LYT
	_kernel_swi_regs regs;
	char *file = "<Befunge$Turtle>";
#else
#ifdef TURTFILE
	char *file = TURTFILE;
#else
	char *file = "Turtle";
#endif
#endif
	t = find_turtle(b);
	/* 'print' current drawing */
	/* Save out as a drawfile for now */
	f = fopen(file,"wb");
	if (f == NULL)
		return EXEC_NEXTINSTR; /* Don't cause error */
	fputstring("Draw",4,0,f);
	fputw(201,f); /* Major version */
	fputw(0,f); /* Minor version */
	fputstring("Befunge",12,32,f);
	fputw(0,f); /* Bounding box */
	fputw(0,f);
	fputw((t->maxx-t->minx)+(129*TURT_SCALE),f);
	fputw((t->maxy-t->miny)+(129*TURT_SCALE),f);
	/* Do paper background */
	/* Must be done as a big rectangle :( */
	if (t->papercol != 0xFFFFFF)
	{
		fputw(2,f); /* Path object */
		fputw(23*4,f); /* Size */
		fputw(0,f); /* Bounding box */
		fputw(0,f);
		fputw((t->maxx-t->minx)+(129*TURT_SCALE),f);
		fputw((t->maxy-t->miny)+(129*TURT_SCALE),f);
		fputwbig(t->papercol,f); /* Fill colour */
		fputw(-1,f); /* Line colour */
		fputw(0,f); /* Line width */
		fputw(0,f); /* Path style */
		fputw(2,f); /* Move component */
		fputw(0,f);
		fputw(0,f);
		fputw(8,f); /* Draw component */
		fputw((t->maxx-t->minx)+(128*TURT_SCALE),f);
		fputw(0,f);
		fputw(8,f); /* Draw component */
		fputw((t->maxx-t->minx)+(128*TURT_SCALE),f);
		fputw((t->maxy-t->miny)+(128*TURT_SCALE),f);
		fputw(8,f); /* Draw component */
		fputw(0,f);
		fputw((t->maxy-t->miny)+(128*TURT_SCALE),f);
		fputw(0,f); /* End path */
	}
	/* Now line objects */
	/* Just do lots of seperate paths for now */
	l = t->lines;
	while (l)
	{
		fputw(2,f); /* Path object */
		fputw(17*4,f); /* Size */
		fputw((MIN(l->sx,l->ex)-t->minx)+(64*TURT_SCALE)-128,f); /* Bounding box */
		fputw((MIN(l->sy,l->ey)-t->miny)+(64*TURT_SCALE)-128,f);
		fputw((MAX(l->sx,l->ex)-t->minx)+(64*TURT_SCALE)+129,f);
		fputw((MAX(l->sy,l->ey)-t->miny)+(64*TURT_SCALE)+129,f);
		fputw(-1,f); /* No fill colour */
		fputwbig(l->col,f); /* Line colour */
		fputw(150,f); /* Line width */
		fputw(0,f); /* Path style */
		fputw(2,f); /* Move component */
		fputw((l->sx-t->minx)+(64*TURT_SCALE),f);
		fputw((l->sy-t->miny)+(64*TURT_SCALE),f);
		fputw(8,f); /* Line component */
		fputw((l->ex-t->minx)+(64*TURT_SCALE),f);
		fputw((l->ey-t->miny)+(64*TURT_SCALE),f);
		fputw(0,f); /* End */
		l = l->next;
	}
	fclose(f);
#ifndef LYT
	regs.r[0] = 18;
	regs.r[1] = (int) file;
	regs.r[2] = 0xAFF;
	_kernel_swi(OS_File,&regs,&regs);
#endif
	return EXEC_NEXTINSTR;
}

int install_turt(bspace *b,bip *ip,int load)
{
	if (load)
		add_turtle(b);
	bef_overloadsem(ip,TURT_FINGER,'L',turt_l,load);
	bef_overloadsem(ip,TURT_FINGER,'R',turt_r,load);
	bef_overloadsem(ip,TURT_FINGER,'H',turt_h,load);
	bef_overloadsem(ip,TURT_FINGER,'F',turt_f,load);
	bef_overloadsem(ip,TURT_FINGER,'B',turt_b,load);
	bef_overloadsem(ip,TURT_FINGER,'P',turt_p,load);
	bef_overloadsem(ip,TURT_FINGER,'C',turt_c,load);
	bef_overloadsem(ip,TURT_FINGER,'N',turt_n,load);
	bef_overloadsem(ip,TURT_FINGER,'D',turt_d,load);
	bef_overloadsem(ip,TURT_FINGER,'T',turt_t,load);
	bef_overloadsem(ip,TURT_FINGER,'E',turt_e,load);
	bef_overloadsem(ip,TURT_FINGER,'A',turt_a,load);
	bef_overloadsem(ip,TURT_FINGER,'Q',turt_q,load);
	bef_overloadsem(ip,TURT_FINGER,'U',turt_u,load);
	bef_overloadsem(ip,TURT_FINGER,'I',turt_i,load);
	return 1;
}

void killprog_turt(bspace *b)
{
	remove_turtle(b);
}


#endif
