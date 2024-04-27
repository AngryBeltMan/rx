#pragma once
#include <argon2_mem.h>
#include <stdint.h>

// The number of parallel lanes for Cache initialization
#define RANDOMX_ARGON_LANES 1
/* The number of 1 KiB Argon2 blocks in the Cache */
#define RANDOMX_ARGON_MEMORY 262144
// The number of Argon2d iterations for Cache initialization
#define RANDOMX_ARGON_ITERATIONS 3
// Argon2 salt
#define RANDOMX_ARGON_SALT "RandomX\x03"

#define RANDOMX_ARGON_SALT_LEN 7

#define RANDOMX_ARGON_VERSION 0x13

// Cache is used for the creation of the hRX_VM.
typedef struct {
    block_t *memory;
} rx_cache_t;

rx_cache_t initCache(char* key, uint8_t key_size);
// Frees the handle and the contents inside the handle
void freeCache(rx_cache_t p_cache);
