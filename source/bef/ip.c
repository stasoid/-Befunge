#ifndef _BEF_IP_C
#define _BEF_IP_C

#include "ip.h"
#include "stack.h"
#include "sem.h"
#include "mem.h"
#include "dynafing.h"
#include "main.h"

/* IP's */

static int maxipid = 0;

bip *bef_newip(bspace *b,bip *clone) /* Create new ip, possibly cloning an existing one */
{
	bip *ip;
	ip = malloc(sizeof(bip));
	ip->next = b->ips;
	ip->prev = (bip *) NULL;
	if (ip->next != NULL)
		ip->next->prev = ip;
	b->ips = ip;
	ip->id = maxipid++;
	ip->haunted = ip->ghost = ip->ghostprev = (bip *) NULL;
	ip->space = b;
	if (clone != NULL)
	{
		bef_cloneinitstack(ip,clone);
		bef_clonesem(b,ip,clone);
		memcpy(ip->pos,clone->pos,sizeof(ip->pos));
		memcpy(ip->delta,clone->delta,sizeof(ip->delta));
		memcpy(ip->origin,clone->origin,sizeof(ip->origin));
		ip->flags = clone->flags;
	}
	else
	{
		bef_initstack(ip);
		bef_initsem(ip);
		memset(ip->pos,0,sizeof(ip->pos));
		memset(ip->delta,0,sizeof(ip->delta));
		memset(ip->origin,0,sizeof(ip->origin));
		ip->flags = 0;
		if ((b->flags & PROG_SCRIPT) && (b->dims > 1))
		{
			/* Skip initial lines */
			while (bef_readspace(b,ip->pos,0) == '#')
				ip->pos[1]++;
		}
	}
	interface_updatespace(b,ip->pos,0);
	return ip;
}

void bef_killip(bspace *b,bip *ip)
{
	if (ip->ghost)
	{
		bef_killip(ip->ghost->space,ip->ghost); /* Kill off whoever's haunting us */
		ip->ghost = (bip *) NULL;
	}
	if (ip->ghostprev)
	{
		dynafing_stacktransplant(ip,ip->ghostprev); /* Move stack down 1 IP */
		ip->ghostprev->ghost = ip->ghost;
	}
	bef_killsem(b,ip);
	bef_killstack(ip);
	if (ip->next != NULL)
		ip->next->prev = ip->prev;
	if (ip->prev != NULL)
		ip->prev->next = ip->next;
	else
		b->ips = ip->next;
	interface_updatespace(b,ip->pos,0);
	do {
		if (b->input_ip == ip)
			b->input_ip = (bip *) NULL; /* Cancel input */
		b = b->haunted; /* Do it for all bspaces in the chain (should only really be the last one with input though) */
	} while (b);
	free(ip);
}

#endif
