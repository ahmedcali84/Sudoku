#include "sudoku.h"

// NOTE: Declare a 2D Board of BOARD_ROWS x BOARD_COLS
CellPool Board[BOARD_ROWS][BOARD_COLS] = {0};

// NOTE: Main Function
int main(void) {
    // Allocate Memory For Grid
    Grid *grid = grid_alloc();

    // Sudoku Grid as a Text File
    const char *file_path = "data/grid1.txt";

    // Read The Sudoku text File and Load it in to the Grid
    if(!read_file(file_path, grid)) {
        return 1;
    }
    
    // Initialize Board , Load Grid into Board 
    InitBoard(grid, Board);

    // Print Initial Board 
    PrintBoard(Board);

    // Search For Valid Numbers for Sudoku Cell and Populate them 
    if(!Search(Board)) {

        // Exit if failed and Free Memories Allocated 
        printf("InValid Board.\n");
        FreeBoard(Board);
        grid_dealloc(grid);
        return 1;
    }

    // Print Solved Board 
    printf("Board Solved.\n");
    PrintBoard(Board);

    // NOTE: Free Allocated Memory
    FreeBoard(Board);
    grid_dealloc(grid);
    return 0;
}
