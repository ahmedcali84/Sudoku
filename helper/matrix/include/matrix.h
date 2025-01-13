#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <assert.h>

#define MATDEF extern //__attribute__((unused)) // Define a macro for static functions and suppresses all warnings assosiated with unused functions defined with MATDEF

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


#endif // MATRIX_H
