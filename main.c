#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include  "./helper/Grid/grid.h"

#define BOARD_ROWS 9
#define BOARD_COLS BOARD_ROWS

typedef struct {
    int value;
} Cell;

typedef struct {
    Cell *cell;
    bool occupied;
} CellPool;

CellPool Board[BOARD_ROWS][BOARD_COLS];

void OccupyCell(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col , int value);

void InitBoard(Grid *g) {
    assert(BOARD_ROWS == g->count && BOARD_COLS == g->items[0]->count);
    for (int i = 0; i < (int)g->count; ++i) {
        for (int j = 0; j < (int) g->items[0]->count; ++j) {
            int n = g->items[i]->buf[j] - '0';
            if (n == 0) {
                Board[i][j].occupied = false;
                Board[i][j].cell = NULL;
            } else {
                OccupyCell(Board, i , j , n);
            }
        }
    }
}

bool CheckCellStatus(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col) {
    if (_Board[row][col].occupied && _Board[row][col].cell != NULL) {
        fprintf(stderr, "Cell %d %d Occupied.\n", row , col);
        return true;
    }
    return false;
}

void OccupyCell(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col , int value) {
    _Board[row][col].cell = malloc(sizeof(Cell));
    if (_Board[row][col].cell == NULL) {
        fprintf(stderr, "ERROR: Failed to Allocate Memory for Cell\n");
        return ;
    }
    _Board[row][col].cell->value = value;
    _Board[row][col].occupied = true;
}

void PrintBoard(CellPool _Board[BOARD_ROWS][BOARD_COLS]) {
    printf("-----------------------------------------\n");
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            if (_Board[i][j].occupied == true) {
                printf("| %d " , _Board[i][j].cell->value);
                if (j == BOARD_COLS - 1) {
                    printf("| %d |" , _Board[i][j].cell->value);
                }
            } else {
                printf("| 0 ");
                if (j == BOARD_COLS - 1) {
                    printf("| 0 |");
                }
            }
        }
        printf("\n");
        printf("-----------------------------------------\n");
    }
}

void FreeCell(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col) {
    free(_Board[row][col].cell);
    _Board[row][col].cell = NULL;
    _Board[row][col].occupied = false;
}

void FreeBoard(CellPool _Board[BOARD_ROWS][BOARD_COLS]) {
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            if(_Board[i][j].occupied) {
                FreeCell(_Board, i , j);
            }
        }
    }
}

bool Constraint(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row, int col) {
    if (CheckCellStatus(_Board, row, col)) {
        return false;
    }

    for (int i = 0; i < BOARD_ROWS; ++i) {
        if (i != row && _Board[i][col].occupied && 
            _Board[i][col].cell->value == _Board[row][col].cell->value) {
            return false; // Duplicate found in the same column
        }

        if (i != col && _Board[row][i].occupied && 
            _Board[row][i].cell->value == _Board[row][col].cell->value) {
            return false; // Duplicate found in the same row
        }
    }
    return false;
}

int RandomInt() {
    return rand() % BOARD_ROWS + 1;
}

void Solve(CellPool _Board[BOARD_ROWS][BOARD_COLS]) {
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            if(!CheckCellStatus(_Board, i , j)) {
                OccupyCell(_Board, i , j , RandomInt());
                if (Constraint(_Board, i , j)) {
                    // FreeCell(_Board, i , j);
                    OccupyCell(_Board, i , j , RandomInt());
                }
            }
        }
    }
}

int main(void) {
    Grid *grid = grid_alloc();
    const char *file_path = "data/grid1.txt";
    if(!read_file(file_path, grid)) {
        return 1;
    }

    InitBoard(grid);
    Solve(Board);
    PrintBoard(Board);

    // NOTE: Free Allocated Memory
    FreeBoard(Board);
    grid_dealloc(grid);
    return 0;
}
