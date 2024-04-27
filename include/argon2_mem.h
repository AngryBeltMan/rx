#pragma once
#include <stdint.h>
#include <argon_core.h>
// size of each block in kbs
#define BLOCK_SIZE 1024
// index the list of memory blocks as if it was a memory matrix. This preprocessor requires _mmatrix to not be a pointer to argon_m_matrix_t.
#define MEM_INDEX(_mmatrix, _row, _col) _mmatrix.memory[((_row) * _mmatrix.col) + (_col)]
typedef union {
    uint64_t v[BLOCK_SIZE / 8];
    uint8_t b[BLOCK_SIZE];
    __uint128_t reg[BLOCK_SIZE / 16];
} block_t;
// Argon memory matrix
typedef struct {
    block_t *memory;
    uint32_t row;
    uint32_t col;
    // length of one slice. (col / 4); floored.
    uint32_t slice_len;
} argon_m_matrix_t;

// allocates memory blocks needed to start hashing with argon2d
// Arguments:
// input - COnfig file that contains important data needed to contruct the memory matrix, like parallelism and mem_cost.
argon_m_matrix_t argon2dAllocMatrix(argon_input_t *input);

// Frees the heap allocated memory matrix inside argon_m_matrix_t
// Arguments:
// mem - The memory matrix that will have its memory freed. Once this happens the memory matrix shouldn't be used again.
//  The attribute memory inside argument mem cannot be NULL or INVALID.
void argon2dFreeMatrix(argon_m_matrix_t mem);
