#include "grid.h"

Grid *grid_alloc() {
    Grid *grid = malloc(sizeof(Grid));
    grid->items = malloc(sizeof(Line*) * INIT_CAPACITY);
    grid->count = 0;
    grid->capacity = INIT_CAPACITY;
    return grid;
}

void append_to_grid(Grid *grid, Line *line) {
    if (grid->count == grid->capacity) {
        // NOTE: Reallocate memory if grid is full
        grid->capacity *= 2;
        grid->items = realloc(grid->items, grid->capacity * sizeof(Line*));
    }
    
    // NOTE: Allocate memory for the new Line and copy its contents
    grid->items[grid->count] = malloc(sizeof(Line));
    memcpy(grid->items[grid->count], line, sizeof(*line));
    grid->count++;
}

void grid_dealloc(Grid *g) {
    for (size_t i = 0; i < g->count; i++) {
        free(g->items[i]->buf);
        free(g->items[i]);
    }
    free(g->items);
    free(g);
}

bool read_file(const char *file_path, Grid *grid) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, READ_FILE_FAILED, file_path);
        return false;
    }

    char buffer[MAX_LINE_COUNT] = {0};
    while(fgets(buffer, sizeof(buffer), file)) {
        buffer[strlen(buffer) - 1] = '\0';
        int len = strlen(buffer);

        Line line;
        line.count  = 0;
        line.capacity = INIT_CAPACITY;
        line.buf = calloc(line.capacity, sizeof(char));
        if (line.buf == NULL) {
            fprintf(stderr, ALLOCATION_FAILED);
            fclose(file);
            return false;
        }

        while(line.count + len + 1 >= line.capacity) {
            line.capacity *= 2;
            char *new_buf = realloc(line.buf, line.capacity);
            if(new_buf == NULL) {
                fprintf(stderr, ALLOCATION_FAILED);
                free(line.buf);
                fclose(file);
                return false;
            }
            line.buf = new_buf;
            free(new_buf);
        }
        memcpy(line.buf + line.count, buffer , len);
        line.count += len;
        append_to_grid(grid, &line);
    }

    fclose(file);
    return true;
}

int *encode(Line *line) {
    int *encoded = malloc(sizeof(int) * line->count);
    if (encoded == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < line->count; i++) {
        encoded[i] = line->buf[i] - '0';
    }

    return encoded;
}

Matrix load_grid_into_matrix(Grid *g) {
    size_t grid_cols = g->items[0]->count;
    size_t grid_rows = g->count;
    Matrix G = CREATE_MATRIX(grid_rows, grid_cols, sizeof(int), TYPE_INT);

    for (size_t row = 0; row < grid_rows; ++row) {
        int *en = encode(g->items[row]);
        for (size_t col = 0; col < grid_cols; ++col) {
            int n = en[col];
            SET_ELEMENT(G, row, col, &n);
        }
        free(en);
    }

    return G;
}

void print_grid(Grid *g) {
    for (size_t i = 0; i < g->count; ++i) {
        for (size_t j = 0; j < g->items[0]->count; ++j) {
            printf("%d ", g->items[i]->buf[j] - '0');
        }
        NEW_LINE();
    }
}

