#ifndef _BEF_IO_C
#define _BEF_IO_C

#ifndef LYT
#include "kernel.h"
#include "swis.h"
#endif

#include "io.h"
#include "../inter/update.h"

/* IO for the IO buffer thing */

void bef_cls(bspace *b)
{
	int c;
	if (nowimp)
	{
		putc(12,stdout);
		return;
	}
	while (b->haunted)
		b = b->haunted;
	for (c=0;c<80*25;c++)
		b->text_buf[c] = 32;
	b->text_x = b->text_y = 0;
	interface_refreshio(b);
}

void bef_cursor(bspace *b,int x,int y)
{
	if (x < 0)
		x = 0;
	else if (x > 79)
		x = 79;
	if (y < 0)
		y = 0;
	else if (y > 24)
		y = 24;
	if (nowimp)
	{
		putc(31,stdout);
		putc(x,stdout);
		putc(y,stdout);
		return;
	}
	while (b->haunted)
		b = b->haunted;
	interface_updateio(b,b->text_x,b->text_y,0);
	b->text_x = x;
	b->text_y = y;
	interface_updateio(b,b->text_x,b->text_y,1);
}

void bef_cursory(bspace *b,int c)
{
	/* Move up/down specified number of lines */
	if (nowimp)
	{
		while (c < 0)
		{
			putc(11,stdout);
			c++;
		}
		while (c > 0)
		{
			putc(10,stdout);
			c--;
		}
		return;
	}
	while (b->haunted)
		b = b->haunted;
	interface_updateio(b,b->text_x,b->text_y,0);
	b->text_y += c;
	if (b->text_y < 0)
		b->text_y = 0;
	else if (b->text_y > 24)
		b->text_y = 24;
	interface_updateio(b,b->text_x,b->text_y,1);
}

void bef_clearline(bspace *b)
{
	/* Clear to end of line */
#ifndef LYT
	_kernel_swi_regs regs;
	if (nowimp)
	{
		do {
			fputc(' ',stdout);
			regs.r[0] = 134;
			_kernel_swi(OS_Byte,&regs,&regs);
		} while (regs.r[1]);
		return;
	}
	while (b->haunted)
		b = b->haunted;
	while (b->text_x)
		bef_putchar(b,' ');
#endif
}

void bef_clearscreen(bspace *b)
{
	/* Clear to end of screen */
#ifndef LYT
	_kernel_swi_regs regs;
	int block[4];
	bef_clearline(b);
	if (nowimp)
	{
		block[0] = 100;
		block[1] = 101;
		block[2] = -1;
		regs.r[0] = (int) block;
		regs.r[1] = (int) block;
		_kernel_swi(OS_ReadVduVariables,&regs,&regs);
		do {
			regs.r[0] = 134;
			_kernel_swi(OS_Byte,&regs,&regs);
			if (!((regs.r[0] == block[0]-1) && (regs.r[1] == block[1]-1)))
				fputc(' ',stdout);
		} while (!((regs.r[0] == block[0]-1) && (regs.r[1] == block[1]-1)));
		return;
	}
	do {
		bef_putchar(b,' ');
	} while (!((b->text_x == 79) && (b->text_y == 24)));
#endif
}

int bef_putchar(bspace *b,int c)
{
	while (b->haunted)
		b = b->haunted;
	if (b->spool)
		fputc(c,b->spool);
	if (nowimp)
		return putchar(c);
	else if (c != 0)
	{
		interface_updateio(b,b->text_x,b->text_y,0);
		if (c != 10) /* Make sure newlines at EOL don't cause 2 lines */
		{
			b->text_buf[b->text_x++ + 80*b->text_y] = c;
		}
		if ((b->text_x >= 80) || (c == 10))
		{
			b->text_y++;
			b->text_x = 0;
			if (b->text_y == 25)
			{
				/* Copy up... */
				for (c=0;c<80*25;c++)
				{
					if (c<80*24)
						b->text_buf[c] = b->text_buf[c+80];
					else
						b->text_buf[c] = 32;
				}
				b->text_y = 24;
				interface_scroll(b);
			}
		}
		interface_updateio(b,b->text_x,b->text_y,1); /* Draw new cursor */
	}
	return 0;
}

void bef_backchar(bspace *b)
{
	/* Try and move back one char */
	while (b->haunted)
		b = b->haunted;
	if (b->spool)
		fputc(8,b->spool);
	if (nowimp)
	{
		putchar(8);
		putchar(32);
		putchar(8);
	}
	else
	{
		interface_updateio(b,b->text_x,b->text_y,0);
		b->text_x--;
		if (b->text_x < 0)
		{
			b->text_x = 79; /* Assume whole line was used */
			b->text_y--;
			if (b->text_y < 0)
				b->text_y = b->text_x = 0; /* Can't go back any further */
		}
		b->text_buf[b->text_x + 80*b->text_y] = 32;
		interface_updateio(b,b->text_x,b->text_y,1);
	}
}

int bef_puts(bspace *b,char *s)
{
	while (b->haunted)
		b = b->haunted;
	if (nowimp)
	{
		if (b->spool)
			fputs(s,b->spool);
		return printf("%s",s);
	}
	else while (*s)
		bef_putchar(b,*(s++));
	return 0;
}

static int is_term(char *c) /* Return whether a string contains a newline */
{
	while (*c)
		if (*(c++) == 10)
			return 1;
	return 0;
}

int bef_getchar(bspace *b,bip *ip,int *waitflag)
{
	int tmp,tmp2,tmp3;
	*waitflag = 0;
	while (b->haunted)
		b = b->haunted;
	if (nowimp)
	{
		if (!(b->flags & PROG_NOPROMPT))
			if (bef_puts(b,"Input a character: ") == EOF)
				return EOF;
		tmp = getchar();
		if ((tmp != EOF) && (b->spool))
			fputc(tmp,b->spool);
		return tmp;
	}
	else
	{
		if (b->input_ip == NULL)
		{
			b->input_ip = ip;
			b->flags |= PROG_INVALIDSTATUS;
			tmp3 = 1; /* Display prompt if needed */
		}
		else
			tmp3 = 0;
		if (b->input_ip == ip)
		{
			if (is_term(b->text_input))
			{
				b->input_ip = (bip *) NULL;
				tmp = b->text_input[0];
				tmp2=0;
				while (b->text_input[tmp2]) /* Gobble 1st char */
				{
					b->text_input[tmp2] = b->text_input[tmp2+1];
					tmp2++;
				}
				b->textbuf_chars--;
				b->flags |= PROG_INVALIDSTATUS;
				return tmp;
			}
		}
	}
	if (tmp3) /* Prompt */
		if (!(b->flags & PROG_NOPROMPT))
			if (bef_puts(b,"Input a character: ") == EOF)
				return EOF;
	*waitflag = 1; /* Wait some more */
	return 0;
}

static int charval(char c,int base)
{
	if ((c >= '0') && (c <= '9'))
		if (base > (c-'0'))
			return c-'0';
	if ((c >= 'A') && (c <= 'Z'))
		if (base > ((c-'A')+16))
			return (c-'A')+16;
	if ((c >= 'a') && (c <= 'z'))
		if (base > ((c-'a')+16))
			return (c-'a')+16;
	return -1;
}

int bef_getint(bspace *b,bip *ip,int base,int *waitflag)
{
	int tmp,tmp2,tmp3,tmp4;
	char buf[64],*prompt;
	switch (base)
	{
		case 10:
			prompt = "Input an integer: ";
			break;
		case 16:
			prompt = "Input a hex number: ";
			break;
		case 8:
			prompt = "Input an octal number: ";
			break;
		case 2:
			prompt = "Input a binary number: ";
			break;
		default:
			sprintf(buf,"Input a number in base %d: ",base);
			prompt = buf;
			break;
	}
	*waitflag = 0;
	while (b->haunted)
		b = b->haunted;
	if (nowimp)
	{
		/* Use same code as FBBI... */
		if (!(b->flags & PROG_NOPROMPT))
			if (bef_puts(b,prompt) == EOF)
				return EOF;
		do {
			tmp = fgetc(stdin);
			if (tmp == EOF)
				return EOF; /* FBBI doesn't do this! naughty! */
			if (charval(tmp,base) != -1)
				ungetc(tmp,stdin);
			else if (b->spool)
				fputc(tmp,b->spool);
		} while (charval(tmp,base) == -1);
		tmp2 = 0;
		do {
			tmp3 = tmp2;
			tmp = fgetc(stdin);
			if (charval(tmp,base) != -1)
				tmp2 = tmp2*base + charval(tmp,base);
		} while ((tmp != EOF) && (charval(tmp,base) != -1) && (tmp2 >= tmp3));
		if (tmp == EOF)
			return EOF;
		if (tmp2 < tmp3)
			return 0x7FFFFFFF;
		return tmp2;
	}
	else if (b->input_ip == NULL)
	{
		b->input_ip = ip;
		b->flags |= PROG_INVALIDSTATUS;
		tmp4 = 1;
	}
	else
		tmp4 = 0;
	if (b->input_ip == ip)
	{
		/* Read input line by line */
		if (is_term(b->text_input))
		{
			tmp=0;
			while (b->text_input[tmp])
			{
				if (charval(b->text_input[tmp],base) != -1)
				{
					tmp2=0;
					tmp3=-1;
					while ((charval(b->text_input[tmp],base) != -1) && (tmp2>=tmp3))
					{
						tmp3 = tmp2;
						tmp2 = (tmp2*base) + charval(b->text_input[tmp],base);
						tmp++;
					}
					if (tmp3>tmp2) /* Overflow? */
					{
						tmp2 = 0x7FFFFFFF;
						tmp--; /* Go back a char */
					}
					/* Now copy from tmp to start of string */
					b->textbuf_chars = 0;
					while (b->text_input[tmp])
					{
						b->text_input[b->textbuf_chars] = b->text_input[tmp];
						tmp++;
						b->textbuf_chars++;
					}
					b->text_input[b->textbuf_chars] = 0; /* Copy terminator  */
					b->input_ip = (bip *) NULL;
					b->flags |= PROG_INVALIDSTATUS;
					return tmp2;
				}
				tmp++;
			}
			/* Failed to find number */
			b->text_input[0] = 0; /* Clear string and try again */
			b->textbuf_chars = 0;
		}
	}
	if (tmp4) /* Prompt */
		if (!(b->flags & PROG_NOPROMPT))
			if (bef_puts(b,prompt) == EOF)
				return EOF;
	*waitflag = 1;
	return 0;
}

int bef_query(bspace *b,bip *ip,int a,int op,int *waitflag)
{
	char tstr[32];
	/* Return result of a division/remainder by 0 request */
	/* Output prompt too */
	*waitflag = 0;
	if (b->flags & PROG_98) /* Don't break the spec by asking the user! */
		return 0;
	if ((op == '/') && ((b->flags & PROG_QUERYDIV0) == 0))
		return b->div0;
	if ((op == '%') && ((b->flags & PROG_QUERYREM0) == 0))
		return b->rem0;
	/* .. else ask */
	while (b->haunted)
		b = b->haunted;
	if (nowimp)
	{
		if (b->spool)
			fprintf(b->spool,"Specify: %d%c0=",a,op);
		if (printf("Specify: %d%c0=",a,op) < 0) /* Always prompt */
		{
			*waitflag = EOF; /* Error */
			return 0;
		}
		/* Use plain scanf */
		if (scanf("%d",&a) != 1)
			*waitflag = EOF;
		else if (b->spool)
			fprintf(b->spool,"%d\n",a);
		return a;
	}
	/* .. else must be multitasking */
	if (b->input_ip == NULL)
	{
		b->input_ip = ip;
		/* Always print prompt */
		sprintf(tstr,"Specify: %d%c0=",a,op);
		bef_puts(b,tstr);
		b->flags |= PROG_INVALIDSTATUS;
	}
	if (b->input_ip == ip)
	{
		/* Got input */
		if (is_term(b->text_input))
		{
			/* Read number */
			if (sscanf(b->text_input,"%d%n",&a,&op) != 1)
			{
				*waitflag = EOF;
				b->input_ip = (bip *) NULL;
				b->flags |= PROG_INVALIDSTATUS;
				return 0;
			}
			/* Discard used chars */
			while ((b->text_input[op] == ' ') || (b->text_input[op] == 10))
				op++;
			b->textbuf_chars = 0;
			while (b->text_input[op])
			{
				b->text_input[b->textbuf_chars] = b->text_input[op];
				op++;
				b->textbuf_chars++;
			}
			b->text_input[b->textbuf_chars] = 0; /* Copy terminator */
			b->input_ip = (bip *) NULL;
			b->flags |= PROG_INVALIDSTATUS;
			return a;
		}
	}
	*waitflag = 1;
	return 0;
}

#endif
