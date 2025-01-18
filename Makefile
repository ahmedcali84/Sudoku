CC=gcc
CFLAGS=-Wall -Wextra -ggdb -I./helper/include
LIB=-Wl,-rpath,./helper/lib -L./helper/lib
SRC=src/main.c src/sudoku.c
OBJ=main
LFLAGS=-l:libhelper.so -lm -ldl -lpthread

all:
	$(CC) $(CFLAGS) $(LIB) $(SRC) -o $(OBJ) $(LFLAGS)
clean:
	$(RM) -r *.dSYM *.o $(OBJ)
