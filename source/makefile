

#	Progs and stuff

LYT = -DLYT
CC = gcc -c -Wall -O2 $(LYT) -o
LINK = gcc -o

WOUMI = <WoumInclude$$Path>

#	Target

^.Befunge:	$(WOUMI)lib.o.limpx $(WOUMI)lib.o.osspr main.o bef/io.o bef/ip.o bef/main.o bef/mem.o bef/prog.o bef/sem.o bef/stack.o inter/file.o inter/funcs.o inter/main.o inter/redraw.o inter/update.o inter/user.o bef/sem/null.o bef/sem/roma.o bef/sem/hrti.o bef/sem/refc.o bef/sem/modu.o bef/sem/orth.o bef/sem/toys.o bef/sem/mode.o bef/sem/turt.o bef/sem/perl.o bef/dynafing.o bef/sem/base.o bef/sem/cpli.o bef/sem/dirf.o bef/sem/file.o bef/sem/frth.o bef/sem/term.o bef/sem/fixp.o
	$(LINK) $@ Woum:lib/limpx.o Woum:lib/osspr.o main.o bef/io.o bef/ip.o bef/main.o bef/mem.o bef/prog.o bef/sem.o bef/stack.o inter/file.o inter/funcs.o inter/main.o inter/redraw.o inter/update.o inter/user.o bef/sem/null.o bef/sem/roma.o bef/sem/hrti.o bef/sem/refc.o bef/sem/modu.o bef/sem/orth.o bef/sem/toys.o bef/sem/mode.o bef/sem/turt.o bef/sem/perl.o bef/dynafing.o bef/sem/base.o bef/sem/cpli.o bef/sem/dirf.o bef/sem/file.o bef/sem/frth.o bef/sem/term.o bef/sem/fixp.o

BefungeLyt:	main.o bef/io.o bef/ip.o bef/main.o bef/mem.o bef/prog.o bef/sem.o bef/stack.o bef/sem/null.o bef/sem/roma.o bef/sem/hrti.o bef/sem/refc.o bef/sem/modu.o bef/sem/orth.o bef/sem/toys.o bef/sem/mode.o bef/sem/turt.o bef/sem/perl.o bef/dynafing.o bef/sem/base.o bef/sem/cpli.o bef/sem/dirf.o bef/sem/file.o bef/sem/frth.o bef/sem/term.o bef/sem/fixp.o
	$(LINK) $@ main.o bef/io.o bef/ip.o bef/main.o bef/mem.o bef/prog.o bef/sem.o bef/stack.o bef/sem/null.o bef/sem/roma.o bef/sem/hrti.o bef/sem/refc.o bef/sem/modu.o bef/sem/orth.o bef/sem/toys.o bef/sem/mode.o bef/sem/turt.o bef/sem/perl.o bef/dynafing.o bef/sem/base.o bef/sem/cpli.o bef/sem/dirf.o bef/sem/file.o bef/sem/frth.o bef/sem/term.o bef/sem/fixp.o


#	Object files

main.o:	main.c
	$(CC) $@ main.c

bef/io.o:	bef/io.c
	$(CC) $@ bef/io.c

bef/ip.o:	bef/ip.c
	$(CC) $@ bef/ip.c

bef/main.o:	bef/main.c
	$(CC) $@ bef/main.c

bef/mem.o:	bef/mem.c
	$(CC) $@ bef/mem.c

bef/prog.o:	bef/prog.c
	$(CC) $@ bef/prog.c

bef/sem.o:	bef/sem.c
	$(CC) $@ bef/sem.c

bef/stack.o:	bef/stack.c
	$(CC) $@ bef/stack.c

inter/file.o:	inter/file.c
	$(CC) $@ inter/file.c

inter/funcs.o:	inter/funcs.c
	$(CC) $@ inter/funcs.c

inter/main.o:	inter/main.c
	$(CC) $@ inter/main.c

inter/redraw.o:	inter/redraw.c
	$(CC) $@ inter/redraw.c

inter/update.o:	inter/update.c
	$(CC) $@ inter/update.c

inter/user.o:	inter/user.c
	$(CC) $@ inter/user.c

bef/sem/null.o:	bef/sem/null.c
	$(CC) $@ bef/sem/null.c

bef/sem/roma.o:	bef/sem/roma.c
	$(CC) $@ bef/sem/roma.c

bef/sem/hrti.o:	bef/sem/hrti.c
	$(CC) $@ bef/sem/hrti.c

bef/sem/refc.o:	bef/sem/refc.c
	$(CC) $@ bef/sem/refc.c

bef/sem/modu.o:	bef/sem/modu.c
	$(CC) $@ bef/sem/modu.c

bef/sem/mode.o:	bef/sem/mode.c
	$(CC) $@ bef/sem/mode.c

bef/sem/orth.o:	bef/sem/orth.c
	$(CC) $@ bef/sem/orth.c

bef/sem/toys.o:	bef/sem/toys.c
	$(CC) $@ bef/sem/toys.c

bef/sem/turt.o:	bef/sem/turt.c
	$(CC) $@ bef/sem/turt.c

bef/sem/perl.o:	bef/sem/perl.c
	$(CC) $@ bef/sem/perl.c

bef/dynafing.o:	bef/dynafing.c
	$(CC) $@ bef/dynafing.c

bef/sem/base.o:	bef/sem/base.c
	$(CC) $@ bef/sem/base.c

bef/sem/cpli.o:	bef/sem/cpli.c
	$(CC) $@ bef/sem/cpli.c

bef/sem/dirf.o:	bef/sem/dirf.c
	$(CC) $@ bef/sem/dirf.c

bef/sem/file.o:	bef/sem/file.c
	$(CC) $@ bef/sem/file.c

bef/sem/frth.o:	bef/sem/frth.c
	$(CC) $@ bef/sem/frth.c

bef/sem/term.o:	bef/sem/term.c
	$(CC) $@ bef/sem/term.c

bef/sem/fixp.o:	bef/sem/fixp.c
	$(CC) $@ bef/sem/fixp.c


#	Source code

main.c:	main.h bef/mem.h bef/ip.h bef/prog.h bef/mem.h version.h
	touch $@

bef/io.c:	bef/io.h inter/update.h
	touch $@

bef/ip.c:	bef/ip.h bef/stack.h bef/sem.h bef/mem.h bef/dynafing.h bef/main.h
	touch $@

bef/main.c:	bef/main.h bef/io.h bef/ip.h bef/mem.h bef/prog.h bef/sem.h bef/stack.h version.h bef/dynafing.h
	touch $@

bef/mem.c:	bef/mem.h
	touch $@

bef/prog.c:	bef/prog.h bef/mem.h bef/ip.h bef/sem.h bef/io.h
	touch $@

bef/sem.c:	bef/sem.h bef/main.h bef/stack.h bef/sem/null.h bef/sem/roma.h bef/sem/hrti.h bef/sem/refc.h bef/sem/modu.h bef/sem/orth.h bef/sem/toys.h bef/sem/mode.h bef/sem/turt.h bef/sem/perl.h bef/dynafing.h bef/sem/base.h bef/sem/cpli.h bef/sem/dirf.h bef/sem/file.h bef/sem/frth.h bef/sem/term.h bef/sem/fixp.h
	touch $@

bef/stack.c:	bef/stack.h
	touch $@

bef/dynafing.c:	bef/dynafing.h bef/main.h bef/stack.h bef/mem.h bef/sem.h bef/prog.h bef/ip.h
	touch $@

inter/file.c:	inter/file.h inter/funcs.h inter/main.h inter/update.h
	touch $@

inter/funcs.c:	inter/funcs.h inter/main.h
	touch $@

inter/main.c:	inter/main.h bef/main.h inter/update.h inter/redraw.h inter/user.h inter/file.h version.h
	touch $@

inter/redraw.c:	inter/redraw.h bef/mem.h inter/main.h bef/stack.h bef/prog.h
	touch $@

inter/update.c:	inter/update.h inter/main.h bef/mem.h inter/funcs.h
	touch $@

inter/user.c:	inter/user.h inter/main.h inter/funcs.h bef/ip.h bef/io.h bef/main.h bef/mem.h inter/update.h bef/prog.h inter/redraw.h
	touch $@

bef/sem/null.c:	bef/sem/null.h
	touch $@

bef/sem/roma.c:	bef/sem/roma.h
	touch $@

bef/sem/hrti.c:	bef/sem/hrti.h
	touch $@

bef/sem/refc.c:	bef/sem/refc.h
	touch $@

bef/sem/modu.c:	bef/sem/modu.h
	touch $@

bef/sem/orth.c:	bef/sem/orth.h
	touch $@

bef/sem/toys.c:	bef/sem/toys.h
	touch $@

bef/sem/mode.c:	bef/sem/mode.h
	touch $@

bef/sem/turt.c:	bef/sem/turt.h
	touch $@

bef/sem/perl.c:	bef/sem/perl.h
	touch $@

bef/sem/base.c:	bef/sem/base.h
	touch $@

bef/sem/cpli.c:	bef/sem/cpli.h
	touch $@

bef/sem/dirf.c:	bef/sem/dirf.h
	touch $@

bef/sem/file.c:	bef/sem/file.h
	touch $@

bef/sem/frth.c:	bef/sem/frth.h
	touch $@

bef/sem/term.c:	bef/sem/term.h
	touch $@

bef/sem/fixp.c:	bef/sem/fixp.h
	touch $@


#	Header files

main.h:	#$(WOUMI)lib.h.limpx
	touch $@

bef/io.h:	main.h
	touch $@

bef/ip.h:	main.h
	touch $@

bef/main.h:	main.h
	touch $@

bef/mem.h:	main.h
	touch $@

bef/prog.h:	main.h
	touch $@

bef/sem.h:	main.h
	touch $@

bef/stack.h:	main.h
	touch $@

inter/file.h:	main.h
	touch $@

inter/funcs.h:	main.h
	touch $@

inter/main.h:	main.h #$(WOUMI)lib.h.osspr
	touch $@

inter/redraw.h:	main.h
	touch $@

inter/update.h:	main.h
	touch $@

inter/user.h:	main.h
	touch $@

bef/sem/null.h:	bef/sem.h bef/main.h
	touch $@

bef/sem/roma.h:	bef/sem.h bef/stack.h
	touch $@

bef/sem/hrti.h:	bef/sem.h bef/stack.h bef/main.h
	touch $@

bef/sem/refc.h:	bef/sem.h bef/stack.h bef/main.h
	touch $@

bef/sem/modu.h:	bef/sem.h bef/stack.h
	touch $@

bef/sem/orth.h:	bef/sem.h bef/stack.h bef/main.h bef/io.h bef/mem.h
	touch $@

bef/sem/toys.h:	bef/sem.h bef/stack.h bef/main.h bef/mem.h
	touch $@

bef/sem/mode.h:	bef/sem.h
	touch $@

bef/sem/turt.h:	bef/sem.h bef/main.h bef/stack.h
	touch $@

bef/sem/perl.h:	bef/sem.h bef/stack.h bef/main.h
	touch $@

bef/dynafing.h:	main.h
	touch $@

bef/sem/base.h:	bef/sem.h bef/stack.h bef/io.h bef/main.h
	touch $@

bef/sem/cpli.h:	bef/sem.h bef/stack.h bef/io.h bef/main.h
	touch $@

bef/sem/dirf.h:	bef/sem.h bef/stack.h bef/main.h
	touch $@

bef/sem/file.h:	bef/sem.h bef/stack.h bef/main.h bef/mem.h
	touch $@

bef/sem/frth.h:	bef/sem.h bef/stack.h bef/mem.h
	touch $@

bef/sem/term.h:	bef/sem.h bef/stack.h bef/io.h
	touch $@

bef/sem/fixp.h:	bef/sem.h bef/stack.h bef/main.h
	touch $@

#	WOUM files

$(WOUMI)lib.o.%:
	dir $(WOUMI)lib
	make $*.o
	dir <Befunge$$Dir>.source

