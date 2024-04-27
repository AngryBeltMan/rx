// Library written by AngryBeltMan on github.com
// Almost all of the implementation is based of the rfc here:
// https://www.rfc-editor.org/rfc/rfc9106.html#name-requirements-language
// Additionally, these sources were used to aid in development:
/* https://github.com/Cpitao/Argon2/blob/main/Argon2.ipynb */
// argon2.online
#include "argon2_mem.h"
#include "argon_core.h"
#include "blake/blake.h"
#include <argon2d.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <opperations.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define ARGON_SLICES 4
// calculates the buffer length if a pointer to argon_input_t is in scope and called input
#define BUFFER_LEN (40 + input->key_len + input->salt_len + input->message_len + input->associated_data_len)
// make this static function un-static when testing in order to test internals
#ifndef TESTING
static
#endif
uint8_t* createBuffer(argon_input_t *input) {
    uint8_t* buffer = malloc(BUFFER_LEN + 1);
    if (buffer == NULL) return NULL;

    memcpy(buffer, &input->parallelism, 4);

    memcpy(buffer + 4, &input->outlen, 4);

    memcpy(buffer + 8, &input->mem_size, 4);

    memcpy(buffer + 12, &input->passes, 4);

    uint32_t a = 0x13;
    memcpy(buffer + 16, &a, 4);

    a = 0;
    memcpy(buffer + 20, &a, 4);
    //append the variable length items below
    //variable used to keep track of what index the salt and other variable length items should be inserted at in the buffer
    size_t buffer_index = 24;

    // append the message/password
    memcpy(buffer + buffer_index, &input->message_len, 4);
    buffer_index += 4;
    memcpy(&buffer[buffer_index], input->message, input->message_len);
    buffer_index += input->message_len;

    // append the salt
    memcpy(buffer + buffer_index, &input->salt_len, 4);
    buffer_index += 4;
    memcpy(&buffer[buffer_index], input->salt, input->salt_len);
    buffer_index += input->salt_len;

    // append the key
    if (input->key_len > 0 ) {
        memcpy(buffer + buffer_index, &input->key_len, 4);
        buffer_index += 4;
        memcpy(&buffer[buffer_index], input->key, input->key_len);
        buffer_index += input->key_len;
    } else {
        memcpy(buffer + buffer_index, &a, 4);
        buffer_index += 4;
    }
    // append the associated data
    if (input->associated_data_len > 0) {
        memcpy(buffer + buffer_index, &input->associated_data_len, 4);
        buffer_index += 4;
        memcpy(&buffer[buffer_index], input->associated_data, input->associated_data_len);
    } else {
        memcpy(buffer + buffer_index, &a, 4);
    }
    return buffer;
}
#ifndef TESTING
static
#endif
argon_res_t argon2InternalHash(char *msg, uint32_t msg_len, uint8_t *out, uint32_t out_len) {
    char *new_msg = malloc(msg_len + 4);
    if (new_msg == NULL)
        return ARGON_BAD_BUFFER;
    memcpy(new_msg, &out_len, 4);
    memcpy(&new_msg[4], msg, msg_len);
    if (out_len <= 64) {
        blake2bHash(new_msg, msg_len + 4, out_len, out);
        free(new_msg);
        return ARGON_RES_OK;
    }
    uint32_t r = ceil((double)out_len / 32.0f) - 2;
    uint8_t prev_block[64];
    blake2bHash(new_msg, msg_len + 4, 64, (uint8_t*)&prev_block );
    memcpy(out, prev_block, 32);
    free(new_msg);

    for (int i = 1; i < r; ++i) {
        blake2bHash((char*)prev_block, 64, 64, prev_block);
        memcpy(&out[i * 32], prev_block, 32);
    }
    uint32_t partial_block_size = out_len - (32*r);
    blake2bHash((char*)prev_block, 64, partial_block_size, prev_block);
    memcpy(out + (r * 32), prev_block, partial_block_size);
    return ARGON_RES_OK;
}
argon_res_t argon2dInitalHash(argon_input_t *input, argon_m_matrix_t memory) {
    // h0 will be used to fill the first 2 blocks
    unsigned char h0[72];
    uint8_t *buffer = createBuffer(input);
    if (buffer == NULL) return ARGON_BAD_BUFFER;
    blake2bHash((char*)buffer, BUFFER_LEN, 64, h0);
    free(buffer);
    h0[65] = 0;
    h0[66] = 0;
    h0[67] = 0;
    for (int32_t i_row = 0; i_row < input->parallelism; ++i_row) {
        memcpy(h0 + 68, &i_row, 4);
        h0[64] = 0;
        argon2InternalHash((char*)h0, 72, MEM_INDEX(memory, i_row, 0).b, 1024);
        h0[64] = 1;
        argon2InternalHash((char*)h0, 72, MEM_INDEX(memory, i_row, 1).b, 1024);
    }
    return ARGON_RES_OK;
}
// used inside helper function p
// similar to the mixing function in Blake2b
// Based on figure 19 in the rfc.
#define GB(a, b, c, d) \
    a = (TRUNC(a) * TRUNC(b) * 2) + a + b; \
    d ^= a;\
    d = ROTATE_RIGHT(d, 32); \
    c = (TRUNC(c) * TRUNC(d) * 2) + c + d; \
    b ^= c;\
    b = ROTATE_RIGHT(b, 24);\
    \
    a = (TRUNC(a) * TRUNC(b) * 2) + a + b; \
    d ^= a;\
    d = ROTATE_RIGHT(d, 16); \
    c = (TRUNC(c) * TRUNC(d) * 2) + c + d; \
    b ^= c;\
    b = ROTATE_RIGHT(b, 63);\

#ifndef TESTING
static
#endif
// Helper function for another helper function called compress.
void p(uint64_t *r0, uint64_t *r1, uint64_t *r2, uint64_t *r3, uint64_t *r4, uint64_t *r5, uint64_t *r6, uint64_t *r7, uint64_t *r8, uint64_t *r9, uint64_t *r10, uint64_t *r11, uint64_t *r12, uint64_t *r13, uint64_t *r14, uint64_t *r15 ) {
    GB(*r0, *r4, *r8, *r12);
    GB(*r1, *r5, *r9, *r13);
    GB(*r2, *r6, *r10, *r14);
    GB(*r3, *r7, *r11, *r15);

    GB(*r0, *r5, *r10, *r15);
    GB(*r1, *r6, *r11, *r12);
    GB(*r2, *r7,  *r8, *r13);
    GB(*r3, *r4,  *r9, *r14);
}

#ifndef TESTING
static
#endif
// Compression function for argon2.
argon_res_t compress(block_t *out, block_t const *x, block_t const *y) {
    // Variable r should be seen as an 8x8 matrix of 128 bit numbers.
    block_t r;
    block_t z;
    // xor x and y and store it into r
    for (uint8_t i = 0; i < BLOCK_SIZE / 16; ++i) {
        r.reg[i] = x->reg[i] ^ y->reg[i];
        z.reg[i] = r.reg[i];
    }
    // function p is applied to each row of the matrix r
    for (uint8_t row = 0; row < 8; ++row) {
        p( &r.v[(row * 16)], &r.v[(row * 16) + 1], &r.v[(row * 16) + 2],&r.v[(row * 16) + 3], &r.v[(row * 16) + 4],&r.v[(row * 16) + 5], &r.v[(row * 16) + 6],&r.v[(row * 16)+ 7],&r.v[(row * 16) + 8], &r.v[(row * 16) + 9],&r.v[(row * 16) + 10], &r.v[(row * 16) + 11],&r.v[(row * 16) + 12], &r.v[(row * 16) + 13],&r.v[(row * 16) + 14], &r.v[(row * 16) + 15]);
    }
    // function p is applied to each column of the matrix r
    for (uint8_t col = 0; col < 8; ++col) {
        p( &r.v[(col*2)     ], &r.v[(col*2) + 1 ],
           &r.v[(col*2) + 16], &r.v[(col*2) + 17],
           &r.v[(col*2) + 32], &r.v[(col*2) + 33],
           &r.v[(col*2) + 48], &r.v[(col*2) + 49],
           &r.v[(col*2) + 64], &r.v[(col*2) + 65],
           &r.v[(col*2) + 80], &r.v[(col*2) + 81],
           &r.v[(col*2) + 96], &r.v[(col*2) + 97],
           &r.v[(col*2) + 112], &r.v[(col*2) + 113]
       );
    }
    XOR_BLOCK((*out), z, r);
    return ARGON_RES_OK;
}

#ifndef TESTING
static
#endif
void getIndexes(uint32_t i, uint32_t j, uint32_t i_pass, argon_input_t *p_input, argon_m_matrix_t memory, uint32_t *out_l, uint32_t *out_z) {
    uint64_t j_1;
    uint64_t j_2;
    uint32_t lane_index;
    assert(j > 0 || i_pass > 1);
    // get the first 32 bits
    j_1 = MEM_INDEX(memory, i, ((j == 0) ? memory.col : j) - 1).v[0] & 0xFFFFFFFFllu;
    // get the last 32 bits
    j_2 = MEM_INDEX(memory, i, ((j == 0) ? memory.col : j) - 1).v[0] >> 32;
    lane_index = j_2 % p_input->parallelism;
    uint32_t w_len;
    uint32_t i_slice = j / (memory.col / 4);
    // first iteration or first slice
    if (i_pass == 1 && i_slice == 0) {
        // set the lane to the current lane
        lane_index = i;
    }
    uint8_t arm;
    // Not the first iteration
    if (i_pass > 1) {
        if (lane_index == i) {
            arm = 4;
            w_len = memory.col - memory.slice_len + (j % memory.slice_len) - 1;
        } else if (j % memory.slice_len == 0) {
            arm = 5;
            w_len = memory.col - memory.slice_len - 1;
        } else {
            arm = 6;
            w_len = memory.col - memory.slice_len;
        }
    } else {
        if (i_slice == 0) {
            arm = 0;
            w_len = (j % memory.slice_len) - 1;
        } else if (lane_index == i) {
            arm = 1;
             w_len = i_slice * memory.slice_len + (j % memory.slice_len) - 1;
        } else if (j % memory.slice_len == 0) {
            arm = 2;
            w_len = i_slice * memory.slice_len - 1;
        } else {
            arm = 3;
            w_len = i_slice * memory.slice_len;
        }
    }
    uint64_t x = (j_1 * j_1) / (UINT32_MAX);
    uint64_t y = (w_len * x) / (UINT32_MAX);
    uint32_t zz = w_len - 1 - y;

    *out_z = (i_pass > 1 && i_slice != 3) ? (memory.slice_len * (i_slice + 1)) : 0;
    *out_z = (*out_z + zz) % memory.col;
    *out_l = lane_index;
}

// hashes the remaining bytes and returns the final outputed hash
argon_res_t argon2dHashRemaining(argon_input_t *input, argon_m_matrix_t memory) {
    uint32_t tick_i;
    uint32_t tick_j;
    uint32_t i_slice;
    uint32_t i;
    uint32_t j;
    for (i_slice = 0; i_slice < 4; ++i_slice) {
        for (i = 0; i < input->parallelism; ++i) {
            j = i_slice * memory.slice_len;
            if (j == 0) j = 2;
            for (; j < (i_slice + 1) * memory.slice_len; ++j) {
                getIndexes(i, j, 1, input, memory,  &tick_i,  &tick_j);
                compress( &MEM_INDEX(memory, i, j), &MEM_INDEX(memory, i, j - 1), &MEM_INDEX(memory, tick_i, tick_j));
            }
        }
    }
    // this block will be used to hold the output of the compression function so it can later be used to xor with a memory block
    block_t output_block;
    for (uint32_t iterations = 2; iterations <= input->passes; ++iterations ) {
        for (i_slice = 0; i_slice < 4; ++i_slice) {
            for (i = 0; i < input->parallelism; ++i) {
                j = i_slice * memory.slice_len;
                for (; j < (i_slice + 1) * memory.slice_len; ++j) {
                    getIndexes(i, j, iterations, input, memory,  &tick_i,  &tick_j);
                    if (j == 0) {
                        compress( &output_block, &MEM_INDEX(memory, i, memory.col - 1), &MEM_INDEX(memory, tick_i, tick_j));
                        XOR_BLOCK(MEM_INDEX(memory, i, 0), MEM_INDEX(memory, i, 0), output_block);
                    } else {
                        compress( &output_block, &MEM_INDEX(memory, i, j - 1), &MEM_INDEX(memory, tick_i, tick_j));
                        XOR_BLOCK(MEM_INDEX(memory, i, j), MEM_INDEX(memory, i, j), output_block);
                    }
                }
            }
        }
    }
    return ARGON_RES_OK;
}
uint8_t* argon2dFinalize(argon_input_t *input, argon_m_matrix_t memory, argon_res_t *res) {
    block_t output_block = memory.memory[memory.col - 1];
    for (int i = 1; i < input->parallelism; ++i) {
        XOR_BLOCK(output_block, MEM_INDEX(memory, i, memory.col - 1), output_block)
    }
    // the final output of the hashing function
    uint8_t *final_out;
    if ((final_out = malloc(input->outlen)) == NULL) {
        *res = ARGON_EXHAUSTED_HEAP;
        return NULL;
    }
    argon2InternalHash((char*)output_block.b, 1024, final_out, input->outlen);
    return final_out;
}
