CC=gcc
CFLAGS=-Wall -Wextra -ggdb -I./helper/matrix/include
LIB=-Wl,-rpath,./helper/matrix/lib -L./helper/matrix/lib
SRC=src/main.c src/sudoku.c helper/Grid/grid.c 
OBJ=main
LFLAGS=-l:libmatrix.so -lm -ldl -lpthread

all:
	$(CC) $(CFLAGS) $(LIB) $(SRC) -o $(OBJ) $(LFLAGS)
clean:
	$(RM) -r *.dSYM *.o $(OBJ)
