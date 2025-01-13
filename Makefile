CC=gcc
CFLAGS=-Wall -Wextra -ggdb -I./helper/matrix/include
LIB=-Wl,-rpath,./helper/matrix/lib -L./helper/matrix/lib
SRC=main.c helper/Grid/grid.c 
OBJ=main
LFLAGS=-l:libmatrix.so -lm -ldl -lpthread

all:
	$(CC) $(CFLAGS) $(LIB) $(SRC) -o $(OBJ) $(LFLAGS)
clean:
	$(RM) -r *.dSYM *.o $(OBJ)
