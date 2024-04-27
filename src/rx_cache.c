#include "argon_core.h"
#include "argon2_mem.h"
#include "argon2d.h"
#include <rx_cache.h>
#include <argon2_mem.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// size of the cache (256 megabytes)
#define CACHE_SIZE (1024 * RANDOMX_ARGON_MEMORY)

// Generate 8 super scalar hash functions needed for the creation of the dataset
static void genSuperScalarHashFns() { }


// creates the cache memory
static block_t* createCacheMem(char *key, uint8_t key_len) {
    // make sure the argon2d blocks are exactly 1 kilobyte
    assert(sizeof(block_t) == 1024);
    // arguments for creating the cache
    argon_input_t in = {
        .message = NULL,
        .message_len = 0,
        .outlen = 0,
        .key = key,
        .key_len = key_len,
        .salt = RANDOMX_ARGON_SALT,
        .salt_len = RANDOMX_ARGON_SALT_LEN,
        .passes = RANDOMX_ARGON_ITERATIONS,
        .parallelism = RANDOMX_ARGON_LANES,
        .mem_size = RANDOMX_ARGON_MEMORY,
        .associated_data = NULL,
        .associated_data_len = 0

    };
    argon_m_matrix_t cache_mem = argon2dAllocMatrix(&in);
    // create the randomx cache using the argon2d version 0x13 hash. Do not finalize the hash, just hash it so that all of the memory blocks in argon are filled.
    int res = ARGON_RES_OK;
    res = argon2dInitalHash(&in, cache_mem);
    if (res != ARGON_RES_OK) goto create_cache_failure;
    res = argon2dHashRemaining(&in, cache_mem);
    if (res != ARGON_RES_OK) goto create_cache_failure;
    return cache_mem.memory;

create_cache_failure:
    // attempt to free any allocated memory
    argon2dFreeMatrix(cache_mem);
    return NULL;
}

rx_cache_t initCache(char *key, uint8_t key_len) {
    rx_cache_t cache;

    // key cannot be greater than 60 bytes
    // key must be provided
    assert(key_len > 0 && "ERROR: Cache key len must be greater than 0");
    assert(key_len <= 60 && "ERROR: Cache key len cannot exceed 60 chars");


    cache.memory = createCacheMem(key, key_len);
    assert(cache.memory && "Womp Womp! Couldn't create the randomx cache");
    return cache;
}

void freeCache(rx_cache_t p_cache) {
    free(p_cache.memory);
}
