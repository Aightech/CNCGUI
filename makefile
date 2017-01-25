
# options de compilation
CC = g++
CCFLAGS = -g -Wall -Wno-switch -Wno-write-strings -I $(LIBDIR)/include
LIBS = -L $(LIBDIR)/lib
LDFLAGS = -lm -lncurses -lpanel -lwiringPi

# fichiers du projet
SRC = main.c guilib.c applib.c cnclib.cpp strlib.c stplib.cpp
OBJ = $(SRC:.c=.o)
EXEC = test 


# règle initiale
all: $(EXEC)
# dépendance des .h
main.o: guilib.h applib.h struct.h strlib.h
guilib.o: guilib.h applib.h strlib.h struct.h
applib.o: applib.h guilib.h strlib.h struct.h
cnclib.o: stplib.h applib.h guilib.h strlib.h struct.h 
stplib.o: stplib.h


# règles de compilation
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<
	
# règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

# règles supplémentaires
clean:
	@rm -rf *.o
rmproper:
	rm -rf $(EXEC) *.o
