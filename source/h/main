#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#ifndef LYT
#include "/WoumInclude:lib/limpx.h"
#endif

#define MAX_DIM 3
#define MEM_CELL_SIZES {16,8,1}
#define STACK_SIZE 256
#define NUM_BREAKPOINTS 8

#define IP_STRINGMODE 1
/* For non-cardinal deltas */
#define IP_FLYING 2
/* MODE fingerprint flags */
#define IP_HOVERMODE 4
#define IP_INVERTMODE 8
#define IP_QUEUEMODE 16
#define IP_SWITCHMODE 32
/* For if the IP is dead but there are still ghosts */
#define IP_DEAD 64
/* For if the pushvalue should be pushed on the stack the next bef_runip */
#define IP_PUSHVALUE 128

#define PROG_98 1
#define PROG_CON 2
#define PROG_FS 4
#define PROG_SCRIPT 8
#define PROG_RUNNING 16
#define PROG_PAUSED 32
#define PROG_WAITFORINPUT 64
#define PROG_INVALIDSTATUS 128
#define PROG_NOPROMPT 256
#define PROG_QUERYDIV0 512
#define PROG_QUERYREM0 1024
#define PROG_TOOLBARINVALID 2048
#define PROG_STATUSALERT 4096
#define PROG_WARN_UNIMPLEMENTED 8192
#define PROG_FINGERPRINT 16384
#define PROG_DYNA_CON 32768
#define PROG_DYNA_KILL 65536
#define PROG_UP_IS_UP 131072
#define PROG_INPUT_PAUSES 262144
#define PROG_MINIFUNGE 524288

#define UP_DIR(b) ((b->flags & PROG_UP_IS_UP) ? 1 : -1)
#define DOWN_DIR(b) ((b->flags & PROG_UP_IS_UP) ? -1 : 1)

/* bef_execinstr return codes */
/* Wait on the spot */
#define EXEC_WAIT 0
/* Move on to next instruction */
#define EXEC_NEXTINSTR 1
/* IP died */
#define EXEC_DEAD 2
/* Don't move on to next instruction, i've already done it */
#define EXEC_IMADEMOVE 3
/* A ghost IP has been spawned */
#define EXEC_GHOSTSPAWN 4

/* Handprint is JBEF */
#define HANDPRINT 0x4A424546

#define MENU_MAIN 1
#define MENU_FUNGE 2

/* fungespace structure definitions */

typedef struct _bstack {
	signed long stack[STACK_SIZE];
	struct _bstack *next;
} bstack;

typedef struct _bstackstack {
	bstack *stack;
	int pos; /* Next free position in current stack */
	int bot; /* Bottom of stack, i.e. lowest pos containing a value (normally 0) */
	struct _bstackstack *next; /* Next in stackstack list */
} bstackstack;

struct _bspace;
struct _bip;

typedef struct _bsemantics {
	struct _bsemantics *next;
	int fing; /* fingerprint it belongs to */
	int (*instr)(struct _bspace *b,struct _bip *ip,int cell); /* Fn pointer */
} bsemantics;

typedef struct _bip {
	int pos[MAX_DIM];
	int delta[MAX_DIM];
	int origin[MAX_DIM];
	int flags;
	bstackstack *stack; /* Top most stack is the one in use */
	struct _bip *next,*prev;
	int id;
	bsemantics *semantics[26]; /* One per instruction, even though it's a silly idea */
	struct _bip *haunted; /* IP we are haunting */
	struct _bip *ghost; /* IP we are getting haunted by */
	struct _bip *ghostprev; /* IP directly before us in haunting list, allowing multiple IPs to haunt the same ghost */
	struct _bspace *space; /* Our bspace */
	int pushvalue;
} bip;

typedef struct _bmem {
	unsigned long mask,twiddle,pos;
	int ends; /* +1 for 'zero', +2 for 'one' */
	struct _bmem *zero, *one;
} bmem;

typedef struct _bspace {
	int dims;
	unsigned long sizes[MAX_DIM]; /* e.g. 80,25,1 for b93. Use 0 for infinite dim */
	int bits;
	int flags;
	bmem space; /* Must have at least one bmem */
	bip *ips;
	struct _bspace *next,*prev;
#ifndef LYT
	limpx_window *win_main,*win_tools,*win_io,*win_ip;
#endif
	int cursor[MAX_DIM];
	int cdir; /* dimension to head in, + 128 if negative dir */
	char text_buf[80*25]; /* Standard PC terminal size */
	int text_x,text_y; /* Cursor locations */
	char text_input[256]; /* Text input buffer */
	int textbuf_chars; /* Number of chars in text buffer */
	bip *input_ip; /* IP currently receiving the input */
	char fname[256]; /* File name */
	int minmem[MAX_DIM],maxmem[MAX_DIM]; /* Min and max locations written to */
	char args[256]; /* Program args - spaces seperate them */
	int div0,rem0; /* Values to use for division/remainder by 0 (or query user if flags are set) */
	int breakflags; /* Bitflags for which breakpoints are active */
	int breaks[NUM_BREAKPOINTS][MAX_DIM];
	FILE *spool;
	struct _bspace *haunted; /* Haunted fungespace */
	int fing; /* Our fingerprint if we are one */
} bspace;

extern bspace *progs;
extern int trace,nowimp;
extern FILE *debuglog;
extern bspace *menu_space;
extern int ip_refresh,disable_refresh;

/* main.c functions */

extern int load_file(bspace *b,char *name,int *by,int flags,int *size);
extern int save_file(bspace *b,char *name,int *from,int *size,int flags);
extern void bef_exit(int code);

/* interface functions */

#ifndef LYT
extern void interface_updatespace(bspace *b,int *pos,int *by);
extern void interface_updateblock(bspace *b,int *from,int *size);
extern void interface_updateio(bspace *b,int x,int y,int center);
extern void interface_refreshio(bspace *b);
extern void init_interface(char *infile,char *dstring,int flags,char *args,char *div0,char *rem0,int bits);
extern void interface_poll();
extern void finish_interface();
extern void interface_statusalert(bspace *b,char *msg,int type);
#else
#define interface_updatespace(a,b,c)
#define interface_updateblock(a,b,c)
#define interface_updateio(a,b,c,d)
#define interface_refreshio(a) 
extern void interface_statusalert(bspace *b,char *msg,int type);
#endif

#endif
