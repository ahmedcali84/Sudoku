#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include  "../helper/Grid/grid.h"

#define BOARD_ROWS 9
#define BOARD_COLS BOARD_ROWS
#define EMPTY 0

typedef struct {
    int value;
} Cell;

typedef struct {
    Cell *cell;
    bool occupied;
} CellPool;

void InitBoard(Grid *g, CellPool _Board[BOARD_ROWS][BOARD_COLS]);
bool CheckCellStatus(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col);
void SetCell(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col , int value);
Cell *GetCell(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col);
void PrintBoard(CellPool _Board[BOARD_ROWS][BOARD_COLS]);
bool ValidBoard(CellPool _Board[BOARD_ROWS][BOARD_COLS]);
int *GetCandidates(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col, int *count);
bool Search(CellPool _Board[BOARD_ROWS][BOARD_COLS]);
void FreeCell(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col);
void FreeBoard(CellPool _Board[BOARD_ROWS][BOARD_COLS]);

#endif // SUDOKU_H
