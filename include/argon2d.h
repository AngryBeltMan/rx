#pragma once
#include "argon2_mem.h"
#include <stdint.h>
#include <argon_core.h>
// Hashed the value with argon2d version 0x13
argon_res_t argon2dHashValue(argon_input_t *input);

// Hash the first two blocks of the output
argon_res_t argon2dInitalHash(argon_input_t *input, argon_m_matrix_t memory);

// Fill in the remaining memory blocks based on the first two blocks.
// In order to get the correct output, call argon2dInitalHash before calling this function.
argon_res_t argon2dHashRemaining(argon_input_t *input, argon_m_matrix_t memory);

// Hashes the filled argon memory blocks to output the final desired output the size of input.outlen.
// In order to get the correct output, call argon2dHashRemaining for calling this function.
uint8_t* argon2dFinalize(argon_input_t *input, argon_m_matrix_t memory, argon_res_t *result);
