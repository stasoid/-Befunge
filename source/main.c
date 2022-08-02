#ifndef _MAIN_C
#define _MAIN_C

#ifndef LYT
#include "kernel.h"
#include "swis.h"
#endif

#include "main.h"
#include "bef/mem.h"
#include "bef/ip.h"
#include "bef/prog.h"
#include "bef/main.h"
#include "version.h"

#ifdef DEBUG
FILE *debuglog;
#endif

/*#define FILEDEBUG */

#ifdef FILEDEBUG
#include <time.h>
#endif

int quiet,trace,nowimp,disable_refresh;

int load_file(bspace *b,char *name,int *by,int flags,int *size)
{
	FILE *f;
	int tmp,tmp2,otmp2;
	int pos[MAX_DIM];
#ifdef FILEDEBUG
	int ft,mt;
	ft=mt=0;
#endif
	if (flags & 1) /* Flag high, use binary */
		f = fopen(name,"rb");
	else
		f = fopen(name,"r");
	if (!f)
		return EOF; /* error! */
	for (tmp=0;tmp<MAX_DIM;tmp++)
	{
		pos[tmp] = 0;
		if (size)
			size[tmp] = 0;
		/* Size returned is: */
		/* (0,1,1) if empty file */
		/* (w,_,_) max. number of characters per line, e.g. 5 for '12345' */
		/* (_,h,_) max number of newlines in layer, +1 */
		/* (_,_,d) number of form feeds +1 */
	}
	otmp2 = 0;
	disable_refresh = 1; /* Turn off fungespace redraws during loads (otherwise it gets v. slow!) */
#ifdef FILEDEBUG
	tmp=clock();
#endif
	tmp2 = fgetc(f);
#ifdef FILEDEBUG
	ft+=clock()-tmp;
	fprintf(stderr,"load_file: %s at %d\n",name,(int) clock());
#endif
	while (tmp2 != EOF)
	{
#ifdef FILEDEBUG
		fprintf(stderr,"read char %d: ",tmp2);
#endif
		if (flags & 1)
		{
#ifdef FILEDEBUG
			fprintf(stderr,"stored\n");
#endif
			if (tmp2 != 32) /* Ignore space still, apparently */
				bef_setspace(b,pos,by,tmp2);
			pos[0]++;
			if (size)
				if (size[0] < pos[0])
					size[0] = pos[0];
		}
		else if (((tmp2 == 10) || (tmp2 == 13)) && (b->dims > 1))
		{
			if (((tmp2 == 10) && (otmp2 != 13)) || ((tmp2 == 13) && (otmp2 != 10))) /* Accept 10, 13, 10-13 and 13-10 */
			{
#ifdef FILEDEBUG
				fprintf(stderr,"newline\n");
#endif
				pos[1]++;
				pos[0] = 0;
				if (size) /* Increase height/depth regardless of whether the lines below contain anything */
					if (size[1] < pos[1])
						size[1] = pos[1];
			}
#ifdef FILEDEBUG
			else
			{
				fprintf(stderr,"skipped newline\n");
			}
#endif
		}
		else if ((tmp2 == 12) && (b->dims > 2))
		{
#ifdef FILEDEBUG
			fprintf(stderr,"form feed\n");
#endif
			pos[2]++;
			pos[1] = 0;
			pos[0] = 0;
			if (size)
				if (size[2] < pos[2])
					size[2] = pos[2];
		}
		else
		{
#ifdef FILEDEBUG
			fprintf(stderr,"stored in ");
			tmp = clock();
#endif
			if (tmp2 != ' ') /* doh! */
				bef_setspace(b,pos,by,tmp2);
#ifdef FILEDEBUG
			mt += clock()-tmp;
			fprintf(stderr,"%dcs\n",clock()-tmp);
#endif
			pos[0]++;
			if (size)
			{
				if (size[0] < pos[0])
					size[0] = pos[0];
			}
		}
		otmp2 = tmp2;
#ifdef FILEDEBUG
		tmp = clock();
#endif
		tmp2 = fgetc(f);
#ifdef FILEDEBUG
		ft += clock()-tmp;
#endif
	}
	fclose(f);
#ifdef FILEDEBUG
	fprintf(stderr,"End at %d, %d file time, %d mem time\n",(int) clock(),ft,mt);
#endif
	if (size)
		for (tmp=1;tmp<MAX_DIM;tmp++)
			size[tmp]++; /* Newlines+1, formfeeds+1, etc. */
	disable_refresh = 0;
	interface_updateblock(b,by,size); /* Redraw it in one go */
	return 0; /* OK */
}

#define X pos[0]
#define Y pos[1]
#define Z pos[2]

int save_file(bspace *b,char *name,int *from,int *size,int flags)
{
	/* Save out to disk... */
	FILE *f;
	int pos[MAX_DIM];
	int sz[MAX_DIM];
	int ox,oy,oz,t;
	f = fopen(name,"w");
	if (!f)
		return EOF; /* Error! */
	if (size == 0)
	{
		size = sz;
		for (t=0;t<b->dims;t++)
			sz[t] = b->maxmem[t]+1;
	}
	oz=0;
	X=Y=Z=0;
	if ((b->dims < 3) || (size[2] > 0)) /* Ensure it only outputs if the size scalar > 0 */
	do
	{
		oy=0;
		if ((b->dims < 2) || (size[1] > 0))
		do
		{
			ox = 0;
			if (size[0] > 0)
			do
			{
#ifdef FILEDEBUG
				fprintf(stderr,"saving %d,%d from %d,%d (",pos[0],pos[1],from[0],from[1]);
#endif
				t = bef_readspace(b,pos,from);
#ifdef FILEDEBUG
				fprintf(stderr,"%d)\n",t);
#endif
				if ((t != 32) || ((flags & 1) == 0))
				{
					while (oz < Z)
					{
						if (fputc(12,f) == EOF)
						{
							fclose(f);
							return EOF;
						}
						oz++;
					}
					while (oy < Y)
					{
						if (fputc(10,f) == EOF)
						{
							fclose(f);
							return EOF;
						}
						oy++;
					}
					while (ox < X)
					{
						if (fputc(32,f) == EOF)
						{
							fclose(f);
							return EOF;
						}
						ox++;
					}
					if (fputc(t,f) == EOF)
					{
						fclose(f);
						return EOF;
					}
					ox++;
				}
				X++;
				t = 1;
				if (X >= size[0])
					t = 0;
			}
			while (t);
			X=0;
			Y++;
			t = 1;
			if (b->dims < 2)
				t = 0; /* Always fail */
			else if (Y >= size[1])
				t = 0;
			if ((t) && ((flags & 1) == 0)) /* More Y left, and not compressed */
			{
				if (fputc(10,f) == EOF) /* .. then output newline */
				{
					fclose(f);
					return EOF;
				}
				oy++;
			}
		}
		while (t);
		Y=0;
		Z++;
		t = 1;
		if (b->dims < 3)
			t = 0;
		else if (Z >= size[2])
			t = 0;
		if ((t) && ((flags & 1) == 0)) /* More Z left, and not compressed */
		{
			if (fputc(12,f) == EOF) /* .. then output formfeed */
			{
				fclose(f);
				return EOF;
			}
			oz++;
		}
	}
	while (t);
	fclose(f);
	return 0; /* OK */
}

#undef X
#undef Y
#undef Z

void run_file(bspace *b)
{
	bip *ip;
	ip = bef_newip(b,(bip *) NULL);
	ip->delta[0] = 1;
	while (b->ips != NULL)
	{
		bef_checkbreakpoints(b); /* Ignore return code since single-tasking */
		bef_runprog(b);
	}
}

void helpfunc()
{
	printf("\
Befunge interpreter %s\n\
\n\
Usage: Befunge [<options>]\n\
Options:\n\
--help          Display this help and then exit\n\
<anything else> The name of a file to load and run, in which case the\n\
                following options can also be used:\n"
#ifndef LYT
"  --nowimp      Disable the WIMP interface (Not needed if already outside the\n\
                desktop, in a taskwindow, etc.)\n"
#endif
"  --quiet       Disable all non-Befunge messages\n\
  --trace       Enable very simple tracing of execution\n\
  --noprompt    Disable input prompts\n\
  --script      The file is a script; the IP will be started at the first line\n\
                which doesn't start with a #\n\
  --warn        Issue warnings on unimplemented instructions being executed\n\
  --d=dims      Set the space dimensions. Eg --d=80,25 would be for Befunge 93,\n\
                --d=*,* for unlimited size Befunge, --d=*,*,* for unlimited\n\
                Trefunge, etc. There is a maximum limit of 3 dimensions.\n\
  --cellsize=n  Set the cell size to n bits. n must be 8 or 32.\n\
  --div0=n      For Befunge-93 programs, set the result of a division by zero to\n\
                the number n (Instead of querying the user).\n\
  --rem0=n      For Befunge-93 programs, set the result of a remainder by zero\n\
                to the number n (Instead of querying the user).\n\
  --98          Enable befunge-98 instructions, and the following options:\n\
  --concurrent  Enable concurrency (multithreading) instructions\n\
  --files       Enable file access instructions\n\
  --args [...]  Arguments following --args are the arguments to be sent to the\n\
                program\n\
  --up-is-up    Make the h instruction set the IPs delta to (0,0,1) (goes\n\
                against the spec)\n\
\n\
Initial setup is suitable for running befunge-93 programs (i.e. 80x25, 8 bit\n\
cells, no extended instructions). For Befunge-98, use --98 --d=*,* --cellsize=32\n\
",BEFUNGE_VERSION);
	exit(0);
}

int main(int argc,char **argv)
{
	char *infile,*n,*dstring,args[256],*div0,*rem0;
	int pos,len,dims,bits,en98,con,file,tmp,script,argpos,runaway,warn,up;
	int sizes[MAX_DIM],noprompt;
	bspace *b;
	ip_refresh = 0;
	progs = (bspace *) NULL;
	nowimp = 0;
	infile = (char *) NULL;
	dims = 2;
	sizes[0] = 80;
	sizes[1] = 25;
	bits = 8;
	dstring = "80,25"; /* Dimension def. string */
	quiet = trace = noprompt = en98 = con = file = script = argpos = runaway = disable_refresh = warn = up = 0;
	div0 = rem0 = 0;
	for (pos=1;pos<argc;pos++)
	{
		len = strlen(argv[pos]);
		if ((argv[pos][0] == '-') && (len > 1))
		{
			if (argv[pos][1] == '-')
			{
				if (strcmp(argv[pos],"--nowimp") == 0)
					nowimp = 1;
				else if (strcmp(argv[pos],"--98") == 0)
					en98 = 1;
				else if ((argv[pos][2] == 'd') && (argv[pos][3] == '='))
				{
					tmp = 0; /* unlimited size */
					sizes[0] = sizes[1] = 1; /* Dummy sizes to make sure the window is OK */
					dims = 1;
					dstring = n = &argv[pos][4];
					while ((*n != 0) && (dims <= MAX_DIM))
					{
						if ((*n >= '0') && (*n <= '9')) /* number */
						{
							tmp = 10*tmp + (*n)-'0';
						}
						else if (*n == '*') /* unlimited size */
							tmp = 0;
						else if (*n == ',') /* next def. */
						{
							sizes[dims-1] = tmp;
							dims++;
							tmp = 0;
						}
						n++;
					}
					/* Also the last entry... */
					sizes[dims-1] = tmp;
					for (tmp=dims;tmp<MAX_DIM;tmp++)
						sizes[tmp] = 1;
				}
				else if (strcmp(argv[pos],"--help") == 0)
					helpfunc();
				else if (strcmp(argv[pos],"--quiet") == 0)
					quiet = 1;
				else if (strcmp(argv[pos],"--trace") == 0)
					trace = 1;
				else if (strcmp(argv[pos],"--noprompt") == 0)
					noprompt = 1;
				else if (strcmp(argv[pos],"--concurrent") == 0)
					con = 1;
				else if (strcmp(argv[pos],"--files") == 0)
					file = 1;
				else if (strcmp(argv[pos],"--script") == 0)
					script = 1;
				else if (strcmp(argv[pos],"--args") == 0)
				{
					argpos = ++pos;
					pos = argc; /* Skip all other args */
				}
				else if (strncmp(argv[pos],"--div0=",7) == 0)
					div0 = &argv[pos][7]; 
				else if (strncmp(argv[pos],"--rem0=",7) == 0)
					rem0 = &argv[pos][7];
				else if (strncmp(argv[pos],"--cellsize=",11) == 0)
				{
					bits = atoi(&argv[pos][11]);
					if ((bits != 8) && (bits != 32))
					{
						printf("*** Unknown --cellsize setting: %s\n",&argv[pos][11]);
						runaway = 1;
					}
				}
				else if (strcmp(argv[pos],"--warn") == 0)
					warn = 1;
				else if (strcmp(argv[pos],"--up-is-up") == 0)
					up = 1;
				else
				{
					printf("*** Unknown option '%s'\n",argv[pos]);
					runaway = 1;
				}
			}
			else
			{
				printf("*** Unknown option '%s'\n",argv[pos]);
				runaway = 1;
			}
		}
		else
			infile = argv[pos];
	}
	if (runaway)
		exit(1);
	if ((argpos) && (argpos < argc))
	{
		/* Translate args into single string */
		/* Simply escapes invalid chars instead of placing them in quotes */
		n=args;
		while (argpos < argc)
		{
			pos=0;
			while (argv[argpos][pos])
			{
				if ((argv[argpos][pos] == '"') || (argv[argpos][pos] == '\\') || (argv[argpos][pos] == ' '))
					*(n++) = '\\'; /* Escape it */
				*(n++) = argv[argpos][pos++];
			}
			*(n++) = ' '; /* Terminator */
			argpos++;
		}
		*n = 0;
	}
	else
		args[0] = 0;
	/* Try to detect wimp state */
	if (nowimp == 0)
#ifdef LYT
		nowimp = 1;
#else
	{
		if (strcmp(getenv("Wimp$State"),"desktop") != 0)
			nowimp = 1;
		else
		{
			_kernel_swi_regs regs;
			regs.r[0] = 0;
			_kernel_swi(TaskWindow_TaskInfo,&regs,&regs);
			if (regs.r[0])
				nowimp = 1;
		}
	}
	if (nowimp)
	{
#endif
		if (infile == NULL)
		{
			if (!quiet) printf("*** --nowimp specified but no file name given!\n");
			exit(1);
		}
		b = bef_newprog(dims,sizes,bits);
		strcpy(b->fname,infile);
		if (load_file(b,infile,(int *) 0,0,(int *) 0) == EOF)
		{
			if (!quiet) printf("*** Unable to load file\n");
			exit(1);
		}
		if (en98)
			b->flags |= PROG_98;
		if (con)
			b->flags |= PROG_CON;
		if (file)
			b->flags |= PROG_FS;
		if (script)
			b->flags |= PROG_SCRIPT;
		if (noprompt)
			b->flags |= PROG_NOPROMPT;
		if (div0)
			b->div0 = atoi(div0);
		else
			b->flags |= PROG_QUERYDIV0;
		if (rem0)
			b->rem0 = atoi(rem0);
		else
			b->flags |= PROG_QUERYREM0;
		if (warn)
			b->flags |= PROG_WARN_UNIMPLEMENTED;
		if (up)
			b->flags |= PROG_UP_IS_UP;
		/* Load args */
		strcpy(b->args,args);
		run_file(b);
		bef_killprog(b);
		if (!quiet) printf("\n*** Exited normally\n");
		return 0;
#ifndef LYT
	}
	/* Else, load wimp... */
	trace = quiet = 0; /* Disable these */
#ifdef DEBUG
	debuglog = fopen("<Befunge$Dir>.Debug","w");
#endif
	init_interface(infile,dstring,(en98*PROG_98)+(con*PROG_CON)+(file*PROG_FS)+(script*PROG_SCRIPT)+(noprompt*PROG_NOPROMPT)+(warn*PROG_WARN_UNIMPLEMENTED)+(up*PROG_UP_IS_UP),args,div0,rem0,bits);
	interface_poll();
	finish_interface();
#ifdef DEBUG
	fprintf(debuglog,"End\n");
	fclose(debuglog);
#endif
	return 0;
#endif
}

void bef_exit(int code)
{
	if (!quiet)
		printf("\n*** Exited normally with code %d\n",code);
	exit(code);
}

#ifdef LYT
/* Full version in inter/update.c */
void interface_statusalert(bspace *b,char *msg,int type)
{
	/* Display on screen */
	if (type & 1)
		printf("\n*** %s",msg);
	else
		return;
	if (type & 2)
	{
		printf(" (Press a key)\n");
		getc(stdin);
	}
	else
		printf("\n");
}
#endif

#endif
