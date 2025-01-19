CC=g++
CFLAGS=-Wall -Wextra -ggdb -I./helper/include -std=c++17
LIB=-Wl,-rpath,./helper/lib -L./helper/lib
SRC=src/animation.cpp src/sudoku.c
SRC1=src/main.c src/sudoku.c
OBJ=gui
OBJ1=main
LFLAGS=-l:libhelper.so -lm -ldl -lpthread -lSDL2
LFLAGS1=-l:libhelper.so -lm -ldl -lpthread

all:
	$(CC) $(CFLAGS) $(LIB) $(SRC) -o $(OBJ) $(LFLAGS)
	$(CC) $(CFLAGS) $(LIB) $(SRC1) -o $(OBJ1) $(LFLAGS1)
clean:
	$(RM) -r *.dSYM *.o $(OBJ)
