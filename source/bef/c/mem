#ifndef _BEF_MEM_C
#define _BEF_MEM_C

#include "mem.h"

/* Debugging info on tree creation */
/*#define MEMDEBUG */
/* Debugging info on bef_getmemspace */
/*#define MEMSPACEDEBUG */
/* Non-existent cells should have codes instead of spaces */
/*#define MEMTREE */

#ifdef MEMTREE
#define MT0 0
#define MT1 1
#define MT2 2
#define MT3 3
#else
#define MT0 32
#define MT1 32
#define MT2 32
#define MT3 32
#endif

static int mem_sizes[3] = MEM_CELL_SIZES; /* Size of the memory cells */

/* Memory */

static long _readspace(bspace *b,unsigned long *pos,bmem *mem)
{
	union {bmem *b; signed long *l; unsigned char *c;} p;
	int in,m,d;
	p.b = mem;
	/* Calculate index */
	if (mem == NULL)
		return MT0;
	in = 0;
	m = 1;
	for (d=0;d<b->dims;d++)
	{
		in+=m*(pos[d] & (mem_sizes[d]-1));
		m*=mem_sizes[d];
	}
	if (b->bits == 8)
		return p.c[in];
	return p.l[in];
}

static bmem *_emptycell(bspace *b)
{
	/* Create an empty cell and return it */
	union {bmem *b; signed long *l; unsigned char *c;} cell;
	int size,dim;
	size=1;
#ifdef MEMDEBUG
	fprintf(stderr,"_emptycell\n");
#endif
	for (dim=0;dim<b->dims;dim++) /* The cells are based around the individual program's dimensions */
		size *= mem_sizes[dim];
	if (b->bits == 8)
	{
		cell.c = malloc(sizeof(unsigned char)*size);
		while (size--)
			cell.c[size] = 32;
	}
	else
	{
		cell.l = malloc(sizeof(signed long)*size);
		while (size--)
			cell.l[size] = 32;
	}
	return cell.b;
}

static void _setspace(bspace *b,unsigned long *pos,bmem *mem,signed long v)
{
	union {bmem *b; signed long *l; unsigned char *c;} p;
	int in,m,d;
	p.b = mem;
#ifdef MEMDEBUG
	fprintf(stderr,"--stored\n\n");
#endif
	/* Calculate index */
	in = 0;
	m = 1;
	for (d=0;d<b->dims;d++)
	{
		in+=m*(pos[d] & (mem_sizes[d]-1));
		m*=mem_sizes[d];
		/* Check min/max */
		if (b->minmem[d] > (int) pos[d])
			b->minmem[d] = (int) pos[d];
		else if (b->maxmem[d] < (int) pos[d]) /* Can't be min and max at same time */
			b->maxmem[d] = (int) pos[d];
	}
	if (b->bits == 8)
	{
		m = p.c[in];
		p.c[in] = v;
		if ((m != p.c[in]) && (!disable_refresh))
			interface_updatespace(b,(int *) pos,(int *) NULL); /* Prevent needless updates */
	}
	else
	{
		m = p.l[in];
		p.l[in] = v;
		if ((m != p.l[in]) && (!disable_refresh))
			interface_updatespace(b,(int *) pos,(int *) NULL);
	}
}

void bef_initcells(bspace *b)
{
	int t;
	/* Create initial cells */
	b->space.mask = 0xFFFFFFFF - ((mem_sizes[b->dims-1]*2)-1);
	b->space.twiddle = mem_sizes[b->dims-1];
	b->space.pos = 0;
	b->space.ends = 3;
	b->space.zero = b->space.one = (bmem *) NULL;
	/* The extra dimensions should be automatically added */
	for (t=0;t<b->dims;t++)
		b->minmem[t]=b->maxmem[t]=0; /* Not written anything */
}

static void kill_recurse(bmem *b,int dim)
{
	if (b == NULL)
		return;
	if ((b->ends & 1) && (dim > 0))
		kill_recurse(b->zero,dim-1); /* Make sure cell blocks aren't recursed */
	else if (b->zero)
		free(b->zero);
	if ((b->ends & 2) && (dim > 0))
		kill_recurse(b->one,dim-1);
	else if (b->one)
		free(b->one);
	free(b);
}

void bef_killcells(bspace *b)
{
	int dim;
	dim = b->dims-1;
	if ((b->space.ends & 1) && (dim > 0))
		kill_recurse(b->space.zero,dim-1); /* Make sure cell blocks aren't recursed */
	else if (b->space.zero)
		free(b->space.zero);
	if ((b->space.ends & 2) && (dim > 0))
		kill_recurse(b->space.one,dim-1);
	else if (b->space.one)
		free(b->space.one);
}

signed long bef_readspace(bspace *b,int *pos,int *by)
{
	/* Return 32 if out of bounds */
	bmem *branch;
	int dim;
	unsigned long rpos[MAX_DIM];
	for (dim=0;dim<b->dims;dim++)
	{
		if (by == NULL)
			rpos[dim] = (unsigned long) pos[dim];
		else
			rpos[dim] = (unsigned long) pos[dim] + by[dim];
		if ((rpos[dim] >= b->sizes[dim]) && (b->sizes[dim] != 0))
			return MT1;
	}
	dim = b->dims-1;
	branch = &b->space;
	do {
		if ((rpos[dim] & branch->mask) != branch->pos)
			return MT2; /* No space for that position */
		/* Else go down a branch... */
		if (rpos[dim] & branch->twiddle)
		{
			/* Go down 'one' */
			if (branch->ends & 2)
				if (dim-- == 0) /* No more dimensions? */
					return _readspace(b,rpos,branch->one);
			/* else just go onto next as normal */
			branch = branch->one;
		}
		else
		{
			/* Go down 'zero' */
			if (branch->ends & 1)
				if (dim-- == 0)
					return _readspace(b,rpos,branch->zero);
			branch = branch->zero;
		}
	} while (branch);
	return MT3;
}

void bef_setspace(bspace *b,int *pos,int *by,signed long v)
{
	/* Search for correct space... */
	bmem *branch;
	bmem *newb;
	int dim;
	unsigned long rpos[MAX_DIM];
	for (dim=0;dim<b->dims;dim++)
	{
		if (by == NULL)
			rpos[dim] = (unsigned long) pos[dim];
		else
			rpos[dim] = (unsigned long) (pos[dim] + by[dim]);
		if ((rpos[dim] >= b->sizes[dim]) && (b->sizes[dim] != 0))
			return; /* Out of bounds */
	}
	dim = b->dims-1;
	branch = &b->space;
	newb = (bmem *) NULL;
#ifdef MEMDEBUG
	fprintf(stderr,"bef_setspace: %d,%d\n",(int) rpos[0],(int) rpos[1]);
#endif
	do {
#ifdef MEMDEBUG
		fprintf(stderr,"Checking against dim %d mask %x, twiddle %x, pos %x, match %x\n",dim,(unsigned int) branch->mask,(unsigned int) branch->twiddle,(unsigned int) branch->pos,(unsigned int) rpos[dim]);
#endif
		if ((rpos[dim] & branch->mask) != branch->pos)
		{
#ifdef MEMDEBUG
			fprintf(stderr,"branch 1\n");
#endif
			/* Insert new branch & empty dimensions */
			/* Need to identify the first bit which differs from the mask */
			newb = malloc(sizeof(bmem));
			newb->mask = branch->mask;
			newb->twiddle = branch->twiddle;
			newb->pos = branch->pos;
			newb->ends = branch->ends;
			newb->zero = branch->zero;
			newb->one = branch->one;
			while ((rpos[dim] & branch->mask) != (branch->pos & branch->mask))
			{
				branch->twiddle *= 2; /* Increase */
				branch->mask ^= branch->twiddle; /* Clear twiddle bit */
			}
			branch->pos = branch->pos & branch->mask; /* Ensure lower bits are forgotten */
#ifdef MEMDEBUG
			fprintf(stderr,"settles at twiddle %x\n",(unsigned int) branch->twiddle);
#endif
			branch->ends = 0;
			/* Now relink */
			if (rpos[dim] & branch->twiddle)
			{
				/* Storing in branch one */
				branch->zero = newb; /* So zero gets the old branch */
				branch->one = newb = malloc(sizeof(bmem));
			}
			else
			{
				branch->one = newb;
				branch->zero = newb = malloc(sizeof(bmem));
			}
			/* Code at end of function will fill in the end bmem, to allow more branches to be made */
		}
		else if (rpos[dim] & branch->twiddle) /* Else go down a branch */
		{
			/* Go down 'one' */
#ifdef MEMDEBUG
			fprintf(stderr,"Going one\n");
#endif
			if (branch->ends & 2)
				if (dim-- == 0) /* No more dimensions? */
				{
					if (!(branch->one))
						branch->one = _emptycell(b);
					_setspace(b,rpos,branch->one,v);
					return;
				}
			/* else just go onto next as normal */
			if (!(branch->one))
			{
				/* Create empty dimensions */
				branch->one = newb = malloc(sizeof(bmem));
			}
			branch = branch->one;
		}
		else
		{
			/* Go down 'zero' */
#ifdef MEMDEBUG
			fprintf(stderr,"Going zero\n");
#endif
			if (branch->ends & 1)
				if (dim-- == 0)
				{
					if (!(branch->zero))
						branch->zero = _emptycell(b);
					_setspace(b,rpos,branch->zero,v);
					return;
				}
			if (!(branch->zero))
			{
				/* Create empty dimensions */
				branch->zero = newb = malloc(sizeof(bmem));
			}
			branch = branch->zero;
		}
	} while (newb == 0);
	/* Fill in newb with the extra dims, starting at dim */
	/* We want terminating branches, so twiddle = 1, mask = 0xFFFFFFFE and pos = rpos[dim] & mask */
	do
	{
#ifdef MEMDEBUG
		fprintf(stderr,"Filling extra dim %d\n",dim);
#endif
		newb->mask = 0xFFFFFFFF - ((mem_sizes[dim]*2)-1);
		newb->twiddle = mem_sizes[dim];
		newb->pos = rpos[dim] & newb->mask;
		newb->ends = 3; /* Is this right? */
		if (rpos[dim] & newb->twiddle)
		{
			/* Go one */
			newb->zero = (bmem *) NULL;
			if (dim-- == 0)
			{
				/* End */
				newb->one = _emptycell(b);
				_setspace(b,rpos,newb->one,v);
				return;
			}
			newb->one = malloc(sizeof(bmem));
			newb = newb->one;
		}
		else
		{
			newb->one = (bmem *) NULL;
			if (dim-- == 0)
			{
				/* End */
				newb->zero = _emptycell(b);
				_setspace(b,rpos,newb->zero,v);
				return;
			}
			newb->zero = malloc(sizeof(bmem));
			newb = newb->zero;
		}
	}
	while (1);
}

static int _lower(bspace *b,int *pos,int dim,bmem *branch,int try_)
{
	/* Search down side 'try_' of branch */
	/* Branch must end */
	if (try_ == 0)
	{
		if (dim == 0)
		{
			if (branch->zero)
				return 1; /* Inside */
			return 0; /* Outside */
		}
		/* Else go down */
		branch = branch->zero;
		dim--;
	}
	else
	{
		if (dim == 0)
		{
			if (branch->one)
				return 1; /* Inside */
			return 0; /* Outside */
		}
		/* Else go down */
		branch = branch->one;
		dim--;
	}
	if (branch == 0)
		return 0;
	do {
		if ((pos[dim] & branch->mask) != branch->pos)
			return 0; /* Nothing there */
		if (pos[dim] & branch->twiddle)
		{
			/* Go down 'one' */
			if (branch->one == 0)
				return 0; /* Nothing here */
			if (branch->ends & 2)
				if (dim-- == 0)
					return 1;
			/* ... else keep going */
			branch = branch->one;
		}
		else
		{
			/* Go down 'zero' */
			if (branch->zero == 0)
				return 0;
			if (branch->ends & 1)
				if (dim-- == 0)
					return 1;
			branch = branch->zero;
		}
	} while (1);
}

static unsigned long _dist(int pos,bmem *branch,int dir,int side)
{
	/* Return distance to cell */
	unsigned long cpos;
	cpos = branch->pos + (side*branch->twiddle);
#ifdef MEMSPACEDEBUG
	fprintf(stderr,"_dist: cpos=%x\n",(unsigned int) cpos);
#endif
	if ((cpos <= (unsigned long) pos) && (cpos+(branch->twiddle) > (unsigned long) pos))
		return 0; /* We're inside it */
	/* ... else must be outside */
	if (dir == 1)
		return cpos-(unsigned long) pos;
	return (unsigned long) pos-(cpos+(branch->twiddle-1));
}

static int _recurse(bspace *b,int *pos,int indim,int dir,bmem *branch,unsigned long *result);

static int _try(bspace *b,int *pos,int indim,int dir,bmem *branch,unsigned long *result,int try_)
{
#ifdef MEMSPACEDEBUG
	fprintf(stderr,"_try: mask=%x, twiddle=%x, pos=%x, ends=%d, zero=%d, one=%d, try=%d\n",(unsigned int) branch->mask,(unsigned int) branch->twiddle,(unsigned int) branch->pos,branch->ends,(branch->zero != 0),(branch->one != 0),try_);
#endif
	if (try_ == 0)
	{
		if (branch->ends & 1)
		{
			if (_lower(b,pos,indim,branch,0))
			{
				/* Calc distance */
				*result = _dist(pos[indim],branch,dir,0);
				return 1;
			}
		}
		else if (branch->zero != 0) /* else recurse */
			return _recurse(b,pos,indim,dir,branch->zero,result);
		/* ... else false */
		return 0;
	}
	/* ... else 1 */
	if (branch->ends & 2)
	{
		if (_lower(b,pos,indim,branch,1))
		{
			/* Calc distance */
			*result = _dist(pos[indim],branch,dir,1);
			return 1;
		}
	}
	else if (branch->one != 0) /* else recurse */
		return _recurse(b,pos,indim,dir,branch->one,result);
	/* ... else false */
	return 0;
}

static int _recurse(bspace *b,int *pos,int indim,int dir,bmem *branch,unsigned long *result)
{
	if (branch == 0)
	{
#ifdef MEMSPACEDEBUG
		fprintf(stderr,"_recurse: Got passed 0!\n");
#endif
		return 0;
	}
#ifdef MEMSPACEDEBUG
	fprintf(stderr,"_recurse: mask=%x, twiddle=%x, pos=%x, ends=%d, zero=%d, one=%d\n",(unsigned int) branch->mask,(unsigned int) branch->twiddle,(unsigned int) branch->pos,branch->ends,(branch->zero != 0),(branch->one != 0));
#endif
	if ((pos[indim] & branch->mask) == branch->pos)
	{
		/* Matches, so try match first */
		if (pos[indim] & branch->twiddle)
		{ /* Go 1 */
			if (branch->ends & 2)
			{
				if (_lower(b,pos,indim,branch,1) == 1)
					return 1; /* Inside match */
			}
			else if (_try(b,pos,indim,dir,branch,result,1))
				return 1; /* else recurse further */
			if (dir == 0) /* Else try other branch */
				return _try(b,pos,indim,dir,branch,result,0);
		}
		else
		{ /* Go 0 */
			if (branch->ends & 1)
			{
				if (_lower(b,pos,indim,branch,0) == 1)
					return 1; /* Inside match */
			}
			else if (_try(b,pos,indim,dir,branch,result,0))
				return 1;
			if (dir == 1) /* Else try other branch */
				return _try(b,pos,indim,dir,branch,result,1);
		}
		/* .. else fail */
		return 0;
	}
	/* ... else doesn't match, so try heading in !dir... */
	if ((dir == 1) && (branch->pos > (unsigned int) pos[indim])) /* Only go down lowest path if we have gone past the target cell */
	{
		if (_try(b,pos,indim,dir,branch,result,0))
			return 1;
		/* ... else go 1 */
		if (_try(b,pos,indim,dir,branch,result,1))
			return 1;
	}
	if ((dir == 0) && (branch->pos+(branch->twiddle*2) <= (unsigned int) pos[indim]))
	{
		if (_try(b,pos,indim,dir,branch,result,1))
			return 1;
		/* ... else 0 */
		if (dir == 0)
			return _try(b,pos,indim,dir,branch,result,0);
	}
	/* else fail */
	return 0;
}

unsigned long bef_getmemspace(bspace *b,int *pos,int indim,int dir)
{
	/* Return amount of free space until next existant memory cell, or 0 if already in one */
	/* Interested in dim indim, dir dir */
	bmem *branch;
	int dim;
	/* Ignore max limits since this is for infinite ones */
	/* Traverse through tree...
	   If indim can't be reached, return infinity
	   Else if indim can be reached and pos can be reached, return 0
	   Else if indim can be reached and pos can't be reached, go back through tree in 0 or 1 dir until cell reached & return distance
	   Else if cell can't be reached, wrap round to other side */
	/* Traverse tree using recursion; use return codes & int ptr to control flow
	   If indim > 0 then subtrees must be searched to check for cells */
	unsigned long result;
	/* First, try and reach indim - once found we won't need to leave it */
	branch = &b->space;
	dim = b->dims-1;
#ifdef MEMSPACEDEBUG
	fprintf(stderr,"bef_getmemspace: Want from %x in dimension %d, dir %d\n",(unsigned int) pos[indim],indim,dir);
#endif
	while (dim != indim) {
		if ((pos[dim] & branch->mask) != branch->pos)
		{
#ifdef MEMSPACEDEBUG
			fprintf(stderr,"Return 0xFFFFFFFF: Can't reach dimension\n\n");
#endif
			return 0xFFFFFFFF; /* Can't reach */
		}
		/* else branch */
		if (pos[dim] & branch->twiddle)
		{
			if (branch->ends & 2)
				dim--; /* Go down dimension, indim check will save us */
			branch = branch->one;
		}
		else
		{
			if (branch->ends & 1)
				dim--;
			branch = branch->zero;
		}
	};
	/* Found dimension
	   Recursive function:
	   if matches then r=recurse()
	     if r=success then return success
	     else if dir=1 and just tried 0 then return recurse(1)
	     else if dir=0 and just tried 1 then return recurse(0)
	     else return fail
	   else
	     if dir=1 then r=recurse(0)
	       if r=success then return success
	       else return recurse(1)
	     if dir=0 then r=recurse(1)
	       if r=success then return success
	       else return recurse(0) */
	result = 0;
#ifdef MEMSPACEDEBUG
	fprintf(stderr,"Found dimension, recursing...\n");
#endif
	if (_recurse(b,pos,indim,dir,branch,&result))
	{
#ifdef MEMSPACEDEBUG
		fprintf(stderr,"Return %x: Found a cell\n\n",(unsigned int) result);
#endif
		return result; /* OK */
	}
	/* ... else failed ... */
	if (dir==1)
	{
#ifdef MEMSPACEDEBUG
		fprintf(stderr,"Return %x: No cell found, so wrap around\n\n",(unsigned int) (0-(unsigned long) pos[indim]));
#endif
		return 0-(unsigned long) pos[indim]; /* Go to max adr */
	}
#ifdef MEMSPACEDEBUG
	fprintf(stderr,"Return %x: No cell found, so wrap around\n\n",(unsigned int) pos[indim]);
#endif
	return (unsigned long) pos[indim]; /* Go to low adr */
}


#endif
