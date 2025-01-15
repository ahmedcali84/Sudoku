#ifndef GRID_IMPLEMENTATION
#define GRID_IMPLEMENTATION

#define MAX_LINE_COUNT 256
#define INIT_CAPACITY 256
#define NEW_LINE() printf("\n")
#define READ_FILE_FAILED "Failed To Read File %s\n"

#include "grid.h"
#include <matrix.h>

typedef struct {
    char *buf;
    size_t count;
    size_t capacity;
} Line;

typedef struct {
    Line **items;
    size_t count;
    size_t capacity;
} Grid;

Grid *grid_alloc();
void append_to_grid(Grid *grid, Line *line);
void grid_dealloc(Grid *g);
bool read_file(const char *file_path, Grid *grid);
int *encode(Line *line);
Matrix load_grid_into_matrix(Grid *g);
void print_grid(Grid *g);

#endif // GRID_IMPLEMENTATION
