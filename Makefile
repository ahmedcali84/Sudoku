CC=g++
CFLAGS=-Wall -Wextra -ggdb -I./helper/include -std=c++17
LIB=-Wl,-rpath,./helper/lib -L./helper/lib
SRC=src/animation.cpp src/sudoku.c
OBJ=main
LFLAGS=-l:libhelper.so -lm -ldl -lpthread -lSDL2

all:
	$(CC) $(CFLAGS) $(LIB) $(SRC) -o $(OBJ) $(LFLAGS)
clean:
	$(RM) -r *.dSYM *.o $(OBJ)
