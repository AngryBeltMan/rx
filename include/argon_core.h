#pragma once
#include <stdint.h>

typedef enum {
    ARGON_RES_OK,
    ARGON_BAD_BUFFER,
    ARGON_EXHAUSTED_HEAP,
} argon_res_t;

typedef struct {
    // in kilobytes, size of output and desired output size
    uint32_t mem_size;
    char* message;
    uint32_t message_len;
    // optional
    char* key;
    uint32_t key_len;
    char* salt;
    uint32_t salt_len;
    // optional
    char* associated_data;

    uint32_t associated_data_len;
    uint32_t parallelism;
    uint32_t tag_length;
    // number of times the memory matrix should be hashed
    uint32_t passes;
    // length of the output
    uint32_t outlen;
} argon_input_t;
