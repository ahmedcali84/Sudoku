#ifndef HELPER_H
#define HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <assert.h>

#define MATDEF extern

/* BEGINNING OF MATRIX FUNCTIONS */

typedef enum {
    TYPE_BOOL,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_SIZE_T,
    TYPE_CHAR,
} Element_Type;

// Matrix Structure and Information
typedef struct {
    void *A;             // Pointer to the array of matrix elements
    size_t nrows;        // Number of rows in the matrix
    size_t ncols;        // Number of columns in the matrix
    size_t element_size; // Element Size
    Element_Type type;   // Element Type
} Matrix;

/*
************** MATRIX OPERATIONS FUNCTIONS DECLARATIONS *******************
*/

MATDEF void GET_ELEMENT(Matrix C, size_t row, size_t col, void *target);                                   // Get Specific Element from the Matrix
MATDEF void SET_ELEMENT(Matrix C, size_t row, size_t col, void *value);                                    // Set a specific element from the matrix to a new value
MATDEF Matrix CREATE_MATRIX(size_t rows, size_t cols, size_t element_size, Element_Type type);             // Creates an empty Matrix With specified parameters
MATDEF Matrix RANDOM_MATRIX(size_t nrows, size_t ncols, size_t element_size, Element_Type type);           // Generates random Matrix(nrows x ncols)
MATDEF void PRINT_MATRIX(const Matrix B, const char *name);                                                      // Prints a Matrix
MATDEF int MATRIX_SHAPE(Matrix A, const char *name);                                                      // Prints the SHAPE of the matrix
MATDEF Matrix MATRIX_ADD(Matrix *A, Matrix *B);                                                            // Adds two Matrices
MATDEF Matrix MATRIX_SUBTRACT(Matrix *A, Matrix *B);                                                       // Subtracts two Matrices
MATDEF Matrix HADAMARD_PRODUCT(Matrix *A, Matrix *B);                                                      // Computes Element-wise Product of Two Matrices
MATDEF Matrix DOT_PRODUCT(Matrix *A, Matrix *B);                                                          // Computes the Dot-Product of two Matrices
MATDEF Matrix TRANSPOSE(Matrix *A);                                                                        // Return TRANSPOSE of Matrix A
MATDEF bool TEST_MATRIX_EQUAL(Matrix A, Matrix B, char *matrix_a, char *matrix_b);                         // Test Matrices for equality
MATDEF Matrix FILL(size_t nrows, size_t ncols, size_t element_size, Element_Type type, void *fill_value);  // Creates A Matrix filled with a specific value (For Development Purposes)
MATDEF void UNLOAD(Matrix *B);                                                                             // Function to free Allocated Memory

// Special Macro Functions
#define PRINT(B) PRINT_MATRIX(B, #B)                                                                       // Macro definition of a special print function
#define SHAPE(A) MATRIX_SHAPE(A, #A)                                                                       // Macro definition of a special SHAPE printing function
#define TEST_MATRIX(A, B) TEST_MATRIX_EQUAL(A, B, #A, #B)                                                  // Special Macro Version of Test Matrix Function also prints the variable-name of the Matrix
#define ALLOCATION_FAILED "Memory Allocation Failed.\n"

/* END OF MATRIX FUNCTIONS */


/* BEGINNING OF GRID FUNCTIONS */

#define MAX_LINE_COUNT 256
#define INIT_CAPACITY 256
#define NEW_LINE() printf("\n")
#define READ_FILE_FAILED "Failed To Read File %s\n"

// Declare the dir for neighbors (left, right, up, down, diagonal)
extern int dir[8][2];

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

MATDEF Grid *grid_alloc();
MATDEF void append_to_grid(Grid *grid, Line *line);
MATDEF void grid_dealloc(Grid *g);
MATDEF bool read_file(const char *file_path, Grid *grid);
MATDEF int *encode(Line *line);
MATDEF int convert_to_int(char c);
MATDEF char convert_to_char(int num);
MATDEF Matrix load_grid_into_matrix(Grid *g);
MATDEF void print_grid(Grid *g);
MATDEF Matrix adjacency_matrix(Matrix grid_matrix);
MATDEF size_t *get_neighbors(Matrix *matrix, size_t row, size_t col, int *num_neighbours);

/* END OF GRID FUNCTIONS */

#ifdef __cplusplus
}
#endif

#endif // HELPER_H
