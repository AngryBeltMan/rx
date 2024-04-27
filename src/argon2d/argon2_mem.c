// Written by AngryBeltMan on github.com
#include <argon2_mem.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
// Wrapper function for argon2dAllocMatrix. Creates a matrix given the rows and column value.
static inline argon_m_matrix_t argon2dAllocMatrixROW_COL(uint32_t row, uint32_t col) {
    argon_m_matrix_t matrix;
    matrix.row = row,
    matrix.col = col,
    matrix.slice_len = col / 4;
    matrix.memory = malloc((row * col) * sizeof(block_t));
    assert(matrix.memory && "Couldn't allocate matrix, heap memory exhausted.");
    return matrix;
}

argon_m_matrix_t argon2dAllocMatrix(argon_input_t *input) {
    uint32_t block_count = 4 * input->parallelism * floor((double)input->mem_size / (4 * input->parallelism));
    uint32_t column_count = block_count / input->parallelism;
    return argon2dAllocMatrixROW_COL(input->parallelism, column_count);
}
inline void argon2dFreeMatrix(argon_m_matrix_t mem) {
    free(mem.memory);
}
