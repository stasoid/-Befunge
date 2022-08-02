#ifndef _BEF_STACK_C
#define _BEF_STACK_C

#include "stack.h"

/* Stack */

static signed long _pop(bstackstack *s) /* Pop from stack given */
{
	bstack *a;
	if (s->stack == NULL)
		return 0; /* No stack to pop from */
	if ((s->stack->next == NULL) && (s->pos == s->bot))
		return 0; /* One empty stack */
	/* Else at least 1 nonempty stack frame */
	s->pos--;
	if (s->pos < 0) /* Emptied top frame, so dispose of it */
	{
		if (s->stack->next == NULL)
		{
			s->pos = s->bot = 0; /* Ensure flagged as empty */
			return 0; /* Prevent last stack from vanishing, to save the memory manager from lots of fiddling */
		}
		a = s->stack->next; /* ... else delete top stack */
		free(s->stack);
		s->stack = a;
		s->pos = STACK_SIZE-1;
	}
	return s->stack->stack[s->pos];
}

static signed long _queuepop(bstackstack *s)
{
	bstack *a,*b;
	signed long val;
	/* Pop from opposite end of stack...
	   Need to recurse
	   Get s->bot from bottom frame
	   Increase s->bot
	   If s->bot reaches STACK_SIZE, we can delete the end stack */
	/* Check empty */
	if (s->stack == NULL) /* Nothing to pop from */
		return 0;
	if ((s->stack->next == NULL) && (s->pos == s->bot)) /* Only one stack, which is empty */
		return 0;
	/* Else must be at least 1 nonempty stack frame */
	a = (bstack *) NULL;
	b = s->stack;
	while (b->next) /* Find end stack */
	{
		a = b;
		b = b->next;
	}
	/* Now b is end stack, a is one before */
	val = b->stack[s->bot++]; /* Get value */
	if (s->bot == STACK_SIZE)
	{
		/* Remove end stack */
		s->bot = 0;
		if (a == NULL)
		{
			/* Last stack become empty */
			/* Like normal pop, leave it in to help memory manager */
			s->pos = s->bot = 0; /* Ensure flagged as empty */
			return 0;
		}
		/* ... else can delete it */
		a->next = (bstack *) NULL;
		free(b);
	}
	/* ... else keep stack */
	return val;
}

static void _push(bstackstack *s,signed long v)
{
	bstack *a;
	if (s->stack == NULL)
	{
		s->stack = malloc(sizeof(bstack));
		s->stack->next = (bstack *) NULL;
	}
	s->stack->stack[s->pos++] = v;
	if (s->pos == STACK_SIZE)
	{
		a = malloc(sizeof(bstack));
		a->next = s->stack;
		s->stack = a;
		s->pos = 0;
	}
}

static void _invertpush(bstackstack *s,signed long v)
{
	/* Push onto last stack... */
	bstack *a;
	if (s->stack == NULL)
	{
		s->stack = malloc(sizeof(bstack));
		s->stack->next = (bstack *) NULL;
		s->pos = 1; /* Bottom value filled */
		s->bot = 0;
		s->stack->stack[0] = v;
		return;
	}
	/* .. else at least one stack frame */
	/* find end one */
	a = s->stack;
	while (a->next)
		a = a->next;
	/* Need to decrease bot and store there */
	s->bot--;
	if (s->bot < 0) /* Create new frame */
	{
		a->next = malloc(sizeof(bstack));
		a->next->next = (bstack *) NULL;
		s->bot = STACK_SIZE-1;
		a = a->next;
	}
	a->stack[s->bot] = v;
}

signed long bef_pop(bip *ip) /* Pop from TOSS */
{
	if (ip->flags & IP_QUEUEMODE)
		return _queuepop(ip->stack);
	return _pop(ip->stack);
}

signed long bef_underpop(bip *ip) /* Pop from SOSS if available */
{
	if (ip->stack->next == NULL)
		return 0;
	if (ip->flags & IP_QUEUEMODE)
		return _queuepop(ip->stack->next);
	return _pop(ip->stack->next);
}

void bef_push(bip *ip,signed long v) /* Push TOSS */
{
	if (ip->flags & IP_INVERTMODE)
		_invertpush(ip->stack,v);
	else
		_push(ip->stack,v);
}

void bef_underpush(bip *ip,signed long v) /* Pop SOSS */
{
	if (ip->stack->next)
	{
		if (ip->flags & IP_INVERTMODE)
			_invertpush(ip->stack->next,v);
		else
			_push(ip->stack->next,v);
	}
}

void bef_popstack(bip *ip)
{
	/* Pop a stack, if possible */
	bstackstack *a;
	bstack *b;
	if (ip->stack->next == NULL)
		return;
	a = ip->stack->next;
	/* Make sure it's been emptied */
	while (ip->stack->stack)
	{
		b = ip->stack->stack->next;
		free(ip->stack->stack);
		ip->stack->stack = b;
	}
	free(ip->stack);
	ip->stack = a;
}

void bef_pushstack(bip *ip)
{
	bstackstack *b;
	b = malloc(sizeof(bstackstack));
	b->next = ip->stack;
	ip->stack = b;
	b->stack = (bstack *) NULL;
	b->pos = b->bot = 0;
}

void bef_initstack(bip *ip)
{
	ip->stack = malloc(sizeof(bstackstack));
	ip->stack->stack = (bstack *) NULL;
	ip->stack->pos = ip->stack->bot = 0;
	ip->stack->next = (bstackstack *) NULL;
}

void bef_cloneinitstack(bip *ip,bip *clone)
{
	/* Copy clone's stack... */
	bstackstack *ip_ss,*c_ss,*par_ss;
	bstack *ip_s,*c_s,*par_s;
	ip->stack = (bstackstack *) NULL;
	c_ss = clone->stack;
	par_ss = (bstackstack *) NULL;
	while (c_ss != NULL)
	{
		ip_ss = malloc(sizeof(bstackstack));
		ip_ss->next = (bstackstack *) NULL;
		ip_ss->stack = (bstack *) NULL;
		if (par_ss == NULL)
			ip->stack = ip_ss;
		else
			par_ss->next = ip_ss;
		ip_ss->pos = c_ss->pos;
		ip_ss->bot = c_ss->bot;
		/* Fill in its stacks */
		par_s = (bstack *) NULL;
		c_s = c_ss->stack;
		while (c_s != NULL)
		{
			ip_s = malloc(sizeof(bstack));
			ip_s->next = (bstack *) NULL;
			if (par_s == NULL)
				ip_ss->stack = ip_s;
			else
				par_s->next = ip_s;
			memcpy(ip_s->stack,c_s->stack,sizeof(c_s->stack));
			par_s = ip_s;
			c_s = c_s->next;
		}
		/* Next stackstack */
		par_ss = ip_ss;
		c_ss = c_ss->next;
	}
}

void bef_killstack(bip *ip)
{
	bstackstack *a;
	bstack *b;
	while (ip->stack != NULL)
	{
		while (ip->stack->stack != NULL)
		{
			b = ip->stack->stack->next;
			free(ip->stack->stack);
			ip->stack->stack = b;
		}
		a = ip->stack->next;
		free(ip->stack);
		ip->stack = a;
	}
}

int bef_stackempty(bip *ip) /* TOSS empty? */
{
	if (ip->stack->stack == NULL)
		return 1;
	if ((ip->stack->stack->next == NULL) && (ip->stack->pos == ip->stack->bot))
		return 1;
	return 0;
}

int bef_stackstackempty(bip *ip) /* TOSS the only stack? */
{
	if (ip->stack->next == NULL)
		return 1;
	return 0;
}

signed long bef_peekstack(bip *ip,int ofs)
{
	/* 0 returns top stack entry */
	bstack *s;
	if (ip->flags & IP_QUEUEMODE)
	{
		/* Nasty hack */
		ofs = bef_stacksize(ip,0)-(ofs+1);
	}
	if (ofs < 0)
		return 0; /* Fix nasty inputs, or queuemode when ofs>=stacksize */
	s = ip->stack->stack;
	ofs = ip->stack->pos-(ofs+1);
	/* Make ofs positive... */
	while ((ofs < 0) && (s->next))
	{
		s = s->next;
		ofs += STACK_SIZE;
	}
	if ((ofs < ip->stack->bot) && (s->next == NULL))
		return 0;
	return s->stack[ofs];
}

int bef_countstacks(bip *ip)
{
	int cnt;
	bstackstack *s;
	cnt=0;
	s = ip->stack;
	while (s)
	{
		cnt++;
		s = s->next;
	}
	return cnt;
}

int bef_stacksize(bip *ip,int s)
{
	/* Top stack is number 0 */
	/* Returns 0 for OOB */
	int cnt;
	bstackstack *ss;
	bstack *ts;
	ss = ip->stack;
	while ((s--) && (ss))
		ss = ss->next;
	if ((!ss) || (!ss->stack))
		return 0;
	ts = ss->stack;
	cnt = ss->pos; /* pos of 0 is empty stack */
	ts = ts->next;
	while (ts)
	{
		cnt+=STACK_SIZE;
		ts = ts->next;
	}
	return cnt-ss->bot;
}

void bef_pushstring(bip *ip,char *s)
{
	int p;
	p=0;
	while (s[p])
		p++;
	bef_push(ip,0);
	while (p--)
		bef_push(ip,s[p]);
}

#endif
