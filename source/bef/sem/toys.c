#ifndef _SEM_TOYS_C
#define _SEM_TOYS_C

#include "toys.h"

static int toys_a(bspace *b,bip *ip,int cell)
{
	int n;
	n = bef_pop(ip);
	cell = bef_pop(ip);
	while (n > 0)
	{
		bef_push(ip,cell);
		n--;
	}
	return EXEC_NEXTINSTR;
}

static int toys_b(bspace *b,bip *ip,int cell)
{
	/* Guess what this is meant to be... */
	int f0,f1;
	f1 = bef_pop(ip);
	f0 = bef_pop(ip);
	bef_push(ip,f0+f1);
	bef_push(ip,f0-f1);
	bef_unimplemented(b,ip,cell);
	return EXEC_NEXTINSTR;
}

static int toys_c(bspace *b,bip *ip,int cell)
{
	/* Low-order fungespace copy */
	/* Assume stack is: */
	/* [source adr] [size] [dest adr] */
	int src[MAX_DIM],size[MAX_DIM],dest[MAX_DIM],ofs[MAX_DIM];
	int t;
	for (t=b->dims-1;t>=0;t--)
		dest[t] = bef_pop(ip);
	for (t=b->dims-1;t>=0;t--)
		size[t] = bef_pop(ip);
	for (t=b->dims-1;t>=0;t--)
		src[t] = bef_pop(ip);
	for (t=0;t<b->dims;t++)
		if (size[t] <= 0)
			return EXEC_NEXTINSTR; /* Nothing to copy */
	ofs[2] = 0;
	do {
		ofs[1] = 0;
		do {
			ofs[0] = 0;
			do {
				bef_setspace(b,dest,ofs,bef_readspace(b,src,ofs));
				ofs[0]++;
			} while (ofs[0] < size[0]);
			ofs[1]++;
		} while ((ofs[1] < size[1]) && (b->dims > 1));
		ofs[2]++;
	} while ((ofs[2] < size[2]) && (b->dims > 2));
	return EXEC_NEXTINSTR;
}

static int toys_d(bspace *b,bip *ip,int cell)
{
	bef_push(ip,bef_pop(ip)-1);
	return EXEC_NEXTINSTR;
}

static int toys_e(bspace *b,bip *ip,int cell)
{
	cell = 0;
	while (bef_stackempty(ip) == 0)
		cell += bef_pop(ip);
	bef_push(ip,cell);
	return EXEC_NEXTINSTR;
}

static int toys_f(bspace *b,bip *ip,int cell)
{
	/* Write stack into space as 2D matrix */
	/* Assume origin relative */
	int pos[MAX_DIM],i,ofs,t;
	if (b->dims < 2)
	{
		bef_unimplemented(b,ip,cell);
		return EXEC_NEXTINSTR;
	}
	for (t=b->dims-1;t>=0;t--)
		pos[t] = bef_pop(ip);
	i = bef_pop(ip);
	ofs = 0;
	while (bef_stackempty(ip) == 0)
	{
		t = bef_pop(ip);
		if (i)
			bef_setspace(b,pos,0,t);
		else if (i < 0)
		{
			pos[0]--;
			ofs--;
			if (ofs <= i)
			{
				pos[0] += ofs;
				ofs = 0;
				pos[1]++;
			}
		}
		else
		{
			pos[0]++;
			ofs++;
			if (ofs >= i)
			{
				pos[0] -= ofs;
				ofs = 0;
				pos[1]++;
			}
		}
	}
	return EXEC_NEXTINSTR;
}

static int toys_g(bspace *b,bip *ip,int cell)
{
	/* Push j groups of i cells */
	/* But j doesn't appear to come from anywhere */
	/* silly specs */
	/* Pop j from stack then */
	int pos[MAX_DIM],i,ofs,j,t;
	if (b->dims < 2)
	{
		bef_unimplemented(b,ip,cell);
		return EXEC_NEXTINSTR;
	}
	for (t=b->dims-1;t>=0;t--)
		pos[t] = bef_pop(ip);
	i = bef_pop(ip);
	j = bef_pop(ip);
	if (i == 0)
		return EXEC_NEXTINSTR;
	ofs = 0;
	while (j != 0)
	{
		t = bef_readspace(b,pos,0);
		bef_push(ip,t);
		if (i < 0)
		{
			pos[0]--;
			ofs--;
			if (ofs <= i)
			{
				pos[0] += ofs;
				ofs = 0;
			}
		}
		else
		{
			pos[0]++;
			ofs++;
			if (ofs >= i)
			{
				pos[0] -= ofs;
				ofs = 0;
			}
		}
		if (ofs == 0)
		{
			if (j < 0)
			{
				j++;
				pos[1]--;
			}
			else
			{
				j--;
				pos[1]++;
			}
		}
	}
	return EXEC_NEXTINSTR;
}

static int toys_h(bspace *b,bip *ip,int cell)
{
	int a,c;
	c = bef_pop(ip);
	a = bef_pop(ip);
	if (c > 0)
		bef_push(ip,a << c);
	else
		bef_push(ip,a >> (0-c));
	return EXEC_NEXTINSTR;
}

static int toys_i(bspace *b,bip *ip,int cell)
{
	bef_push(ip,bef_pop(ip)+1);
	return EXEC_NEXTINSTR;
}

static int toys_j(bspace *b,bip *ip,int cell)
{
	/* Shift fungespace column north/south */
	/* Assume no wrapping, just like get/put */
	/* Make sure it's shifted from the correct end though */
	int shift;
	int pos[MAX_DIM];
	int t;
	if (b->dims < 2)
	{
		bef_unimplemented(b,ip,cell);
		return EXEC_NEXTINSTR;
	}
	shift = bef_pop(ip);
	if (b->dims == 1)
		return EXEC_NEXTINSTR;
	for (t=0;t<b->dims;t++)
		pos[t] = ip->pos[t];
	if (shift > 0)
	{
		/* starting from the max pos work down to the min pos */
		pos[1] = b->maxmem[1];
		while (pos[1] >= b->minmem[1])
		{
			cell = bef_readspace(b,pos,0);
			pos[1] += shift;
			bef_setspace(b,pos,0,cell);
			pos[1] -= shift;
			pos[1]--;
		}
		pos[1] += shift; /* Clear the end bit */
		while (pos[1] >= b->minmem[1])
		{
			bef_setspace(b,pos,0,32);
			pos[1]--;
		}
	}
	else if (shift < 0)
	{
		/* start from min pos and go up to max */
		pos[1] = b->minmem[1];
		while (pos[1] <= b->maxmem[1])
		{
			cell = bef_readspace(b,pos,0);
			pos[1] += shift;
			bef_setspace(b,pos,0,cell);
			pos[1] -= shift;
			pos[1]++;
		}
		pos[1] += shift; /* Clear the end bit */
		while (pos[1] <= b->maxmem[1])
		{
			bef_setspace(b,pos,0,32);
			pos[1]++;
		}
	}
	return EXEC_NEXTINSTR;
}

static int toys_k(bspace *b,bip *ip,int cell)
{
	/* High-order fungespace copy */
	/* Assume stack is: */
	/* [source adr] [size] [dest adr] */
	int src[MAX_DIM],size[MAX_DIM],dest[MAX_DIM],ofs[MAX_DIM];
	int t;
	for (t=b->dims-1;t>=0;t--)
		dest[t] = bef_pop(ip);
	for (t=b->dims-1;t>=0;t--)
		size[t] = bef_pop(ip);
	for (t=b->dims-1;t>=0;t--)
		src[t] = bef_pop(ip);
	for (t=0;t<b->dims;t++)
		if (size[t] <= 0)
			return EXEC_NEXTINSTR; /* Nothing to copy */
	ofs[2] = size[2]-1;
	do {
		ofs[1] = size[1]-1;
		do {
			ofs[0] = size[0]-1;
			do {
				bef_setspace(b,dest,ofs,bef_readspace(b,src,ofs));
				ofs[0]--;
			} while (ofs[0] >= size[0]);
			ofs[1]--;
		} while ((ofs[1] >= size[1]) && (b->dims > 1));
		ofs[2]--;
	} while ((ofs[2] >= size[2]) && (b->dims > 2));
	return EXEC_NEXTINSTR;
}

static int toys_l(bspace *b,bip *ip,int cell)
{
	/* Get cell to left of IP */
	int ofs[MAX_DIM];
	if (b->dims < 2)
	{
		bef_unimplemented(b,ip,cell);
		return EXEC_NEXTINSTR;
	}
	ofs[0] = ip->delta[1];
	ofs[1] = -ip->delta[0];
	for (cell=2;cell<b->dims;cell++)
		ofs[cell] = ip->delta[cell];
	bef_push(ip,bef_readspace(b,ip->pos,ofs));
	return EXEC_NEXTINSTR;
}

static int toys_m(bspace *b,bip *ip,int cell)
{
	/* Low order move */
	/* Assume stack is: */
	/* [source adr] [size] [dest adr] */
	int src[MAX_DIM],size[MAX_DIM],dest[MAX_DIM],ofs[MAX_DIM];
	int t;
	for (t=b->dims-1;t>=0;t--)
		dest[t] = bef_pop(ip);
	for (t=b->dims-1;t>=0;t--)
		size[t] = bef_pop(ip);
	for (t=b->dims-1;t>=0;t--)
		src[t] = bef_pop(ip);
	for (t=0;t<b->dims;t++)
		if (size[t] <= 0)
			return EXEC_NEXTINSTR; /* Nothing to move */
	ofs[2] = 0;
	do {
		ofs[1] = 0;
		do {
			ofs[0] = 0;
			do {
				bef_setspace(b,dest,ofs,bef_readspace(b,src,ofs));
				bef_setspace(b,src,ofs,32); /* Clear old pos */
				ofs[0]++;
			} while (ofs[0] < size[0]);
			ofs[1]++;
		} while ((ofs[1] < size[1]) && (b->dims > 1));
		ofs[2]++;
	} while ((ofs[2] < size[2]) && (b->dims > 2));
	return EXEC_NEXTINSTR;
}

static int toys_n(bspace *b,bip *ip,int cell)
{
	/* Negate top value */
	bef_push(ip,-bef_pop(ip));
	return EXEC_NEXTINSTR;
}

static int toys_o(bspace *b,bip *ip,int cell)
{
	/* Shift fungespace row east/west */
	int shift;
	int pos[MAX_DIM];
	int t;
	shift = bef_pop(ip);
	for (t=0;t<b->dims;t++)
		pos[t] = ip->pos[t];
	if (shift > 0)
	{
		/* starting from the max pos work down to the min pos */
		pos[0] = b->maxmem[0];
		while (pos[0] >= b->minmem[0])
		{
			cell = bef_readspace(b,pos,0);
			pos[0] += shift;
			bef_setspace(b,pos,0,cell);
			pos[0] -= shift;
			pos[0]--;
		}
		pos[0] += shift; /* Clear the end bit */
		while (pos[0] >= b->minmem[0])
		{
			bef_setspace(b,pos,0,32);
			pos[0]--;
		}
	}
	else if (shift < 0)
	{
		/* start from min pos and go up to max */
		pos[0] = b->minmem[0];
		while (pos[0] <= b->maxmem[0])
		{
			cell = bef_readspace(b,pos,0);
			pos[0] += shift;
			bef_setspace(b,pos,0,cell);
			pos[0] -= shift;
			pos[0]++;
		}
		pos[0] += shift; /* Clear the end bit */
		while (pos[0] <= b->maxmem[0])
		{
			bef_setspace(b,pos,0,32);
			pos[0]++;
		}
	}
	return EXEC_NEXTINSTR;
}

static int toys_p(bspace *b,bip *ip,int cell)
{
	/* Push product of all stack values */
	cell = 1;
	while (bef_stackempty(ip) != 0)
		cell *= bef_pop(ip);
	bef_push(ip,cell);
	return EXEC_NEXTINSTR;
}

static int toys_q(bspace *b,bip *ip,int cell)
{
	/* Push value behind IP */
	int ofs[MAX_DIM],t;
	for (t=0;t<b->dims;t++)
		ofs[t] = -ip->delta[t];
	bef_setspace(b,ip->pos,ofs,bef_pop(ip));
	return EXEC_NEXTINSTR;
}

static int toys_r(bspace *b,bip *ip,int cell)
{
	/* Pick cell to right of IP */
	int ofs[MAX_DIM];
	if (b->dims < 2)
	{
		bef_unimplemented(b,ip,cell);
		return EXEC_NEXTINSTR;
	}
	ofs[0] = -ip->delta[1];
	ofs[1] = ip->delta[0];
	for (cell=2;cell<b->dims;cell++)
		ofs[cell] = ip->delta[cell];
	bef_push(ip,bef_readspace(b,ip->pos,ofs));
	return EXEC_NEXTINSTR;
}

static int toys_s(bspace *b,bip *ip,int cell)
{
	/* Fill fungespace with cell */
	/* Assume stack is */
	/* cell [src] [size] */
	int src[MAX_DIM],size[MAX_DIM],ofs[MAX_DIM];
	int t;
	for (t=b->dims-1;t>=0;t--)
		size[t] = bef_pop(ip);
	for (t=b->dims-1;t>=0;t--)
		src[t] = bef_pop(ip);
	cell = bef_pop(ip);
	for (t=0;t<b->dims;t++)
		if (size[t] <= 0)
			return EXEC_NEXTINSTR; /* Nothing to fill */
	ofs[2] = 0;
	do {
		ofs[1] = 0;
		do {
			ofs[0] = 0;
			do {
				bef_setspace(b,src,ofs,cell);
				ofs[0]++;
			} while (ofs[0] < size[0]);
			ofs[1]++;
		} while ((ofs[1] < size[1]) && (b->dims > 1));
		ofs[2]++;
	} while ((ofs[2] < size[2]) && (b->dims > 2));
	return EXEC_NEXTINSTR;
}

static int toys_t(bspace *b,bip *ip,int cell)
{
	/* Pop dimension number and _|m it */
	int dim,t;
	dim = bef_pop(ip);
	if ((dim >= b->dims) || (dim < 0))
	{
		bef_reflectip(b,ip); /* Error */
		return EXEC_NEXTINSTR;
	}
	for (t=0;t<b->dims;t++)
		ip->delta[t] = 0;
	if (bef_pop(ip) == 0)
		ip->delta[dim] = 1;
	else
		ip->delta[dim] = -1;
	ip->flags &= ~IP_FLYING;
	return EXEC_NEXTINSTR;
}

static int toys_u(bspace *b,bip *ip,int cell)
{
	/* one-shot '?' - change dir then set cell to it */
	char *plus = ">vl",*minus = "<^h";
	int t,t2;
	for (t=0;t<b->dims;t++)
		ip->delta[t] = 0;
	if (rand() >= RAND_MAX/2)
	{
		t2 = -1;
		plus = minus;
	}
	else
		t2 = 1;
	t = (int) ((((float) rand())/RAND_MAX)*((float) b->dims));
	ip->delta[t] = t2;
	ip->flags &= ~IP_FLYING;
	bef_setspace(b,ip->pos,0,plus[t]);
	return EXEC_NEXTINSTR;
}

static int toys_v(bspace *b,bip *ip,int cell)
{
	/* high-order move */
	/* Assume stack is: */
	/* [source adr] [size] [dest adr] */
	int src[MAX_DIM],size[MAX_DIM],dest[MAX_DIM],ofs[MAX_DIM];
	int t;
	for (t=b->dims-1;t>=0;t--)
		dest[t] = bef_pop(ip);
	for (t=b->dims-1;t>=0;t--)
		size[t] = bef_pop(ip);
	for (t=b->dims-1;t>=0;t--)
		src[t] = bef_pop(ip);
	for (t=0;t<b->dims;t++)
		if (size[t] <= 0)
			return EXEC_NEXTINSTR; /* Nothing to copy */
	ofs[2] = size[2]-1;
	do {
		ofs[1] = size[1]-1;
		do {
			ofs[0] = size[0]-1;
			do {
				bef_setspace(b,dest,ofs,bef_readspace(b,src,ofs));
				bef_setspace(b,src,ofs,32);
				ofs[0]--;
			} while (ofs[0] >= size[0]);
			ofs[1]--;
		} while ((ofs[1] >= size[1]) && (b->dims > 1));
		ofs[2]--;
	} while ((ofs[2] >= size[2]) && (b->dims > 2));
	return EXEC_NEXTINSTR;
}

static int toys_w(bspace *b,bip *ip,int cell)
{
	/* pop vector then value */
	/* if vec+ofs=val do nothing */
	/* else if <val push val+vec (inverted) & back up ip 1 delta (i.e. wait for =val in concurrent funge) */
	/* else if >val act like r */
	int vec[MAX_DIM],stackval,spaceval,t;
	for (t=b->dims-1;t>=0;t--)
		vec[t] = bef_pop(ip);
	stackval = bef_pop(ip);
	spaceval = bef_readspace(b,vec,ip->origin);
	if (spaceval < stackval)
	{
		/* Wait */
		bef_push(ip,stackval);
		for (t=0;t<b->dims;t++)
			bef_push(ip,vec[t]);
		return EXEC_WAIT;
	}
	else if (spaceval > stackval)
		bef_reflectip(b,ip);
	return EXEC_NEXTINSTR;
}

static int toys_x(bspace *b,bip *ip,int cell)
{
	/* Inc IP's X coord */
	ip->pos[0]++;
	if ((b->sizes[0]) && (ip->pos[0] >= (int) b->sizes[0]))
		ip->pos[0] -= b->sizes[0];
	return EXEC_NEXTINSTR;
}

static int toys_y(bspace *b,bip *ip,int cell)
{
	/* Inc IP's Y coord */
	/* Assume plain reflect if unefunge */
	if (b->dims < 2)
	{
		bef_reflectip(b,ip);
		return EXEC_NEXTINSTR;
	}
	ip->pos[1]++;
	if ((b->sizes[1]) && (ip->pos[1] >= (int) b->sizes[1]))
		ip->pos[1] -= b->sizes[1];
	return EXEC_NEXTINSTR;
}

static int toys_z(bspace *b,bip *ip,int cell)
{
	/* Inc IP's Z coord */
	if (b->dims < 3)
	{
		bef_reflectip(b,ip);
		return EXEC_NEXTINSTR;
	}
	ip->pos[2]++;
	if ((b->sizes[2]) && (ip->pos[2] >= (int) b->sizes[2]))
		ip->pos[2] -= b->sizes[2];
	return EXEC_NEXTINSTR;
}


int install_toys(bspace *b,bip *ip,int load)
{
	bef_overloadsem(ip,TOYS_FINGER,'A',toys_a,load);
	bef_overloadsem(ip,TOYS_FINGER,'B',toys_b,load);
	bef_overloadsem(ip,TOYS_FINGER,'C',toys_c,load);
	bef_overloadsem(ip,TOYS_FINGER,'D',toys_d,load);
	bef_overloadsem(ip,TOYS_FINGER,'E',toys_e,load);
	bef_overloadsem(ip,TOYS_FINGER,'F',toys_f,load);
	bef_overloadsem(ip,TOYS_FINGER,'G',toys_g,load);
	bef_overloadsem(ip,TOYS_FINGER,'H',toys_h,load);
	bef_overloadsem(ip,TOYS_FINGER,'I',toys_i,load);
	bef_overloadsem(ip,TOYS_FINGER,'J',toys_j,load);
	bef_overloadsem(ip,TOYS_FINGER,'K',toys_k,load);
	bef_overloadsem(ip,TOYS_FINGER,'L',toys_l,load);
	bef_overloadsem(ip,TOYS_FINGER,'M',toys_m,load);
	bef_overloadsem(ip,TOYS_FINGER,'N',toys_n,load);
	bef_overloadsem(ip,TOYS_FINGER,'O',toys_o,load);
	bef_overloadsem(ip,TOYS_FINGER,'P',toys_p,load);
	bef_overloadsem(ip,TOYS_FINGER,'Q',toys_q,load);
	bef_overloadsem(ip,TOYS_FINGER,'R',toys_r,load);
	bef_overloadsem(ip,TOYS_FINGER,'S',toys_s,load);
	bef_overloadsem(ip,TOYS_FINGER,'T',toys_t,load);
	bef_overloadsem(ip,TOYS_FINGER,'U',toys_u,load);
	bef_overloadsem(ip,TOYS_FINGER,'V',toys_v,load);
	bef_overloadsem(ip,TOYS_FINGER,'W',toys_w,load);
	bef_overloadsem(ip,TOYS_FINGER,'X',toys_x,load);
	bef_overloadsem(ip,TOYS_FINGER,'Y',toys_y,load);
	bef_overloadsem(ip,TOYS_FINGER,'Z',toys_z,load);
	return 1;
}

#endif
