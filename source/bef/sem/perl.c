#ifndef _SEM_PERL_C
#define _SEM_PERL_C

#ifdef LYT
#include "../../lyt.h"
#endif

#include "perl.h"

static int perl_do(bspace *b,bip *ip,int cell)
{
	char pname[256],oname[256],cmd[256],*n;
	FILE *f;
	int tmp,result;
	switch(cell)
	{
		case 'S': bef_push(ip,1); break;
		case 'E':
		case 'I':
			tmpnam(pname);
/*			sprintf(pname,"<Befunge$Dir>.perlprog"); */
			f = fopen(pname,"w");
			if (f == 0) /* Can't open file */
			{
				while (bef_pop(ip)) {}; /* Ensure string is gobbled */
				bef_reflectip(b,ip);
				return EXEC_NEXTINSTR;
			}
			tmpnam(oname);
/*			sprintf(oname,"<Befunge$Dir>.perlout"); */
			fprintf(f,"open(OFILE,'>%s');\n",oname);
			fprintf(f,"print OFILE eval('");
			do {
				cell = bef_pop(ip);
				if ((cell == '\'') || (cell == '\\'))
					fputc('\\',f);
				if (cell)
					fputc(cell,f);
			} while (cell);
			fprintf(f,"');\n");
			fprintf(f,"close OFILE;\n");
			fclose(f);
#ifndef LYT
			sprintf(cmd,"<Perl$Dir>.perl %s",pname);
			if (nowimp)
				result = system(cmd);
			else
			{
				if (LimpX_StartTask(cmd))
					result = 0;
				else if (atoi((getenv("Sys$ReturnCode") ? : "1")) != 0) /* Return code of 1 if return code var not found (should never happen though) */
					result = -2;
				else
					result = 0;
			}
#else
#ifdef PERL
			sprintf(cmd,PERL " %s",pname);
			result = system(cmd);
#else
			result = -2;
#endif
#endif
			remove(pname);
			if (result == -2)
			{
				remove(oname);
				bef_reflectip(b,ip);
				return EXEC_NEXTINSTR;
			}
			/* Now get result */
			f = fopen(oname,"rb");
			if (f == NULL)
			{
				remove(oname);
				bef_reflectip(b,ip);
				return EXEC_NEXTINSTR;
			}
			if (cell == 'I')
			{
				fscanf(f,"%d",&cell);
				bef_push(ip,cell);
			}
			else
			{
				fseek(f,0,SEEK_END); /* Go to end of file */
				tmp = ftell(f); /* Get length */
				n = malloc(sizeof(char)*(tmp+1));
				fseek(f,0,SEEK_SET); /* Go to start */
				n[tmp] = 0;
				while (tmp)
					n[--tmp] = fgetc(f); /* Load file backwards */
				bef_push(ip,0); /* Push string */
				while (n[tmp])
					bef_push(ip,n[tmp++]);
				free(n);
			}
			fclose(f);
			remove(oname);
			break;
	}
	return EXEC_NEXTINSTR;
}

int install_perl(bspace *b,bip *ip,int load)
{
	if ((b->flags & PROG_FS) == 0)
		return 0;
	bef_overloadsem(ip,PERL_FINGER,'S',perl_do,load);
	bef_overloadsem(ip,PERL_FINGER,'E',perl_do,load);
	bef_overloadsem(ip,PERL_FINGER,'I',perl_do,load);
	return 1;
}

#endif
