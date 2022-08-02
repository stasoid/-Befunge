#ifndef _SEM_PERL_H
#define _SEM_PERL_H

#include "../sem.h"
#include "../main.h"
#include "../stack.h"

#define PERL_FINGER 0x5045524c

extern int install_perl(bspace *b,bip *ip,int load);

#endif
