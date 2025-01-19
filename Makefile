#Compiler
CC=g++

# NOTE: GUI VERSION
PKGS=sdl2 SDL2_ttf
CFLAGS=-Wall -Wextra -ggdb -I./helper/include -std=c++17 `pkg-config --cflags $(PKGS)`
LIB=-Wl,-rpath,./helper/lib -L./helper/lib
SRC=src/animation.cpp src/sudoku.c
OBJ=gui
LFLAGS=-l:libhelper.so -lm -ldl -lpthread `pkg-config --libs $(PKGS)` 

# NOTE: TERMINAL VERSION
CFLAGS1=-Wall -Wextra -ggdb -I./helper/include -std=c++17
LIB1=-Wl,-rpath,./helper/lib -L./helper/lib
SRC1=src/main.c src/sudoku.c
OBJ1=main
LFLAGS1=-l:libhelper.so -lm -ldl -lpthread


all:
	$(CC) $(CFLAGS) $(LIB) $(SRC) -o $(OBJ) $(LFLAGS)
	$(CC) $(CFLAGS1) $(LIB1) $(SRC1) -o $(OBJ1) $(LFLAGS1)
clean:
	$(RM) -r *.dSYM *.o $(OBJ) $(OBJ1)
