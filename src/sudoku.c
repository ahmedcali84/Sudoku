#include "sudoku.h"

// NOTE: Function to Initialize the Board and Populate it with Values Read from the Grid
void InitBoard(Grid *g, CellPool _Board[BOARD_ROWS][BOARD_COLS]) {
    assert(BOARD_ROWS == g->count && BOARD_COLS == g->items[0]->count);
    for (int i = 0; i < (int)g->count; ++i) {
        for (int j = 0; j < (int) g->items[0]->count; ++j) {
            int n = g->items[i]->buf[j] - '0';
            if (n == EMPTY) {
                _Board[i][j].occupied = false;
                _Board[i][j].cell = NULL;
            } else {
                SetCell(_Board, i , j , n);
            }
        }
    }
}

// NOTE: Function to Check Whether a cell from the Board is Occupied or Not
bool CheckCellStatus(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col) {
    if (_Board[row][col].occupied && _Board[row][col].cell != NULL) {
        return true;
    }
    return false;
}

// NOTE: Function to Set a Cell from the Board to a Specific Value
void SetCell(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col , int value) {
    _Board[row][col].cell = (Cell *)malloc(sizeof(Cell));
    if (_Board[row][col].cell == NULL) {
        fprintf(stderr, "ERROR: Failed to Allocate Memory for Cell\n");
        return ;
    }
    _Board[row][col].cell->value = value;
    _Board[row][col].occupied = true;
}

// NOTE: Function that returns a Cell from the Board
Cell *GetCell(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col) {
    return _Board[row][col].cell;
}

// NOTE: Function to print the Board in the terminal
void PrintBoard(CellPool _Board[BOARD_ROWS][BOARD_COLS]) {
    // Print the top border of the board
    printf("+");
    for (int i = 0; i < BOARD_COLS; ++i) {
        printf("----");
    }
    printf("+\n");

    // Print each row of the board
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            if (_Board[i][j].occupied) {
                printf("| %d ", _Board[i][j].cell->value);
            } else {
                printf("| 0 ");
            }
        }
        printf("|\n");

        // Print the row separator
        printf("+");
        for (int j = 0; j < BOARD_COLS; ++j) {
            printf("----");
        }
        printf("+\n");
    }
}

// NOTE: Funtion that Return Whether the Board is Valid or Not
bool ValidBoard(CellPool _Board[BOARD_ROWS][BOARD_COLS]) {
    // Check for duplicates in Columns
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS - 1; ++j) {
            if (_Board[i][j].occupied && _Board[i][j + 1].occupied) {
                if (GetCell(_Board, i , j)->value == GetCell(_Board, i , j + 1)->value) {
                    return false;
                }
            }
        }
    }

    // Check for duplicates in Rows
    for (int i = 0; i < BOARD_ROWS - 1; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            if (_Board[i][j].occupied && _Board[i + 1][j].occupied) {
                if (GetCell(_Board, i , j)->value == GetCell(_Board, i + 1, j)->value) {
                    return false;
                }
            }
        }
    }

    // Check for duplicates in the Sub-Grids
    for (int m = 0; m < BOARD_ROWS; ++m) {
        for (int n = 0; n < BOARD_COLS; ++n) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (_Board[i + m][j + n].occupied) {
                        int CurrentValue = GetCell(_Board, i + m , n + j)->value;
                        for (int x = 0; x < 3; ++x) {
                            for (int y = 0; y < 3; ++y) {
                                if (x == i && y == j) continue;
                                if(_Board[x + m][y + n].occupied && GetCell(_Board, x + m , y + n)->value == CurrentValue) {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

// NOTE: Functions that return an array  of potential Candidates for a cell in the Board
int *GetCandidates(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col, int *count) {
    int *Candidates = (int *)malloc(sizeof(int) * BOARD_COLS*1);
    for (int i = 0; i < BOARD_COLS; ++i) {
        Candidates[i] = i + 1;
    }

    for (int i = 0; i < BOARD_ROWS; ++i) {
        if (CheckCellStatus(_Board, row, i)) {
            int CurrentValue = GetCell(_Board, row , i)->value;
            for (int k = 0; k < BOARD_COLS; ++k) {
                if (Candidates[k] == CurrentValue) {
                    Candidates[k] = 0;
                }
            }
        }

        if (CheckCellStatus(_Board, i , col)) {
            int CurrentValue = GetCell(_Board, i , col)->value;
            for (int k = 0; k < BOARD_COLS; ++k) {
                if (Candidates[k] == CurrentValue) {
                    Candidates[k] = 0;
                }
            }
        }
    }

    int GRID = sqrt(BOARD_ROWS);
    int SubGridRow = (row / GRID) * GRID;
    int SubGridCol = (col / GRID) * GRID;

    for (int i = SubGridRow; i < SubGridRow + 3; ++i) {
        for (int j = SubGridCol; j < SubGridCol + 3; ++j) {
            if(CheckCellStatus(_Board, i , j)) {
                int CurrentValue = GetCell(_Board, i , j)->value;
                for (int k = 0; k < BOARD_COLS; ++k) {
                    if (Candidates[k] == CurrentValue) {
                        Candidates[k] = 0;
                    }
                }
            }
        }
    }

    int *ValidCandidates = (int *) malloc(sizeof(int) * BOARD_COLS);
    if (ValidCandidates == NULL) {
        fprintf(stderr, "Memory Allocation Failed.\n");
        free(Candidates);
        return NULL;
    }

    int Count = 0;
    for (int i = 0; i < BOARD_COLS; ++i) {
        if (Candidates[i] != 0) {
            ValidCandidates[Count++] = Candidates[i];
        }
    }
    *count = Count;

    free(Candidates);
    return ValidCandidates;
}

// NOTE: Function that Solves the board recursively
bool Search(CellPool _Board[BOARD_ROWS][BOARD_COLS]) {
    if (ValidBoard(_Board)) {
        return true;
    }

    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            if(!CheckCellStatus(_Board, i , j)) {
                int Count;
                int *Candidates = GetCandidates(_Board, i , j, &Count);
                for (int k = 0; k < Count; ++k) {
                    SetCell(_Board, i , j , Candidates[k]);

                    bool IsSolved = Search(_Board);
                    if(IsSolved) {
                        return true;
                    }
                    FreeCell(_Board, i , j);
                }
                free(Candidates);
                return false;
            }
        }
    }

    return true;
}

// NOTE: Function that frees a cell
void FreeCell(CellPool _Board[BOARD_ROWS][BOARD_COLS], int row , int col) {
    free(_Board[row][col].cell);
    _Board[row][col].cell = NULL;
    _Board[row][col].occupied = false;
}

// NOTE: Function that frees the Board
void FreeBoard(CellPool _Board[BOARD_ROWS][BOARD_COLS]) {
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            if(_Board[i][j].occupied) {
                FreeCell(_Board, i , j);
            }
        }
    }
}
