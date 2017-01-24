
# options de compilation
CC = g++
CCFLAGS = -Wall -Wno-switch -Wno-write-strings -I $(LIBDIR)/include
LIBS = -L $(LIBDIR)/lib
LDFLAGS = -lm -lncurses -lpanel -lwiringPi

# fichiers du projet
SRC = main.c guilib.c 
OBJ = $(SRC:.c=.o)
EXEC = test


# règle initiale
all: $(EXEC) clean

# dépendance des .h
main.o: guilib.h applib.h struct.h
guilib.o: guilib.h strlib.h struct.h


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
