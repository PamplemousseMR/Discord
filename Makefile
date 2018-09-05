CC = gcc
CCFLAGS_STD = -Wall -Werror -O3 -pedantic
CCFLAGS = $(CC) $(CCFLAGS_STD) -ansi -o
CCFLAGS_MAIN = $(CC) $(CCFLAGS_STD) -o
CCLIB = -lcurses

COMMUNDIR = Commun/

all :
	make customer
	make coordinator

CUSTOMEROUTDIR = CustomerOFiles/
CUSTOMERDIR = Customer/
CUSTOMEREXEC = customer_exe

CUSTOMEROBJETS = $(CUSTOMEROUTDIR)main.o $(CUSTOMEROUTDIR)ncurses.o $(CUSTOMEROUTDIR)option.o $(CUSTOMEROUTDIR)getch.o $(CUSTOMEROUTDIR)signal.o

customer : customer_directory customer_compile

customer_directory :
	mkdir -p $(CUSTOMEROUTDIR)

customer_compile : $(CUSTOMEROBJETS)
	$(CCFLAGS_MAIN) $(CUSTOMEREXEC) $(CUSTOMEROBJETS) $(CCLIB)

$(CUSTOMEROUTDIR)main.o : $(CUSTOMERDIR)main.c $(CUSTOMERDIR)option.h $(CUSTOMERDIR)ncurses.h $(CUSTOMERDIR)signal.h $(CUSTOMERDIR)getch.h
	$(CCFLAGS) $(CUSTOMEROUTDIR)main.o -c $(CUSTOMERDIR)main.c

$(CUSTOMEROUTDIR)getch.o : $(CUSTOMERDIR)getch.c $(CUSTOMERDIR)getch.h
	$(CCFLAGS) $(CUSTOMEROUTDIR)getch.o -c $(CUSTOMERDIR)getch.c

$(CUSTOMEROUTDIR)signal.o : $(CUSTOMERDIR)signal.c $(CUSTOMERDIR)signal.h
	$(CCFLAGS) $(CUSTOMEROUTDIR)signal.o -c $(CUSTOMERDIR)signal.c

$(CUSTOMEROUTDIR)ncurses.o : $(CUSTOMERDIR)ncurses.c $(CUSTOMERDIR)ncurses.h
	$(CCFLAGS) $(CUSTOMEROUTDIR)ncurses.o -c $(CUSTOMERDIR)ncurses.c

$(CUSTOMEROUTDIR)option.o : $(CUSTOMERDIR)option.c $(CUSTOMERDIR)option.h
	$(CCFLAGS) $(CUSTOMEROUTDIR)option.o -c $(CUSTOMERDIR)option.c

COORDINATOROUTDIR = CoordinatorOFiles/
COORDINATORDIR = Coordinator/
COORDINATOREXEC = coordinator_exe

COORDINATOROBJETS = $(COORDINATOROUTDIR)main.o $(COORDINATOROUTDIR)option.o $(COORDINATOROUTDIR)signal.o

coordinator : coordinator_directory coordinator_compile

coordinator_directory :
	mkdir -p $(COORDINATOROUTDIR)

coordinator_compile : $(COORDINATOROBJETS)
	$(CCFLAGS_MAIN) $(COORDINATOREXEC) $(COORDINATOROBJETS)

$(COORDINATOROUTDIR)main.o : $(COORDINATORDIR)main.c $(COORDINATORDIR)option.h $(COORDINATORDIR)signal.h
	$(CCFLAGS) $(COORDINATOROUTDIR)main.o -c $(COORDINATORDIR)main.c

$(COORDINATOROUTDIR)signal.o : $(COORDINATORDIR)signal.c $(COORDINATORDIR)signal.h
	$(CCFLAGS) $(COORDINATOROUTDIR)signal.o -c $(COORDINATORDIR)signal.c

$(COORDINATOROUTDIR)option.o : $(COORDINATORDIR)option.c $(COORDINATORDIR)option.h
	$(CCFLAGS) $(COORDINATOROUTDIR)option.o -c $(COORDINATORDIR)option.c

clean :
	rm -f $(COORDINATOROBJETS)
	rm -f $(CUSTOMEROBJETS)
	rm -r -f $(CUSTOMEROUTDIR)
	rm -f $(COORDINATOREXEC)
	rm -f $(CUSTOMEREXEC)
	rm -r -f $(COORDINATOROUTDIR)

