#pragma once

// randomx config params

// Dataset base size in bytes
#define RANDOMX_DATASET_BASE_SIZE 2147483648
#define RANDOMX_DATASET_EXTRA_SIZE 33554368

#include <stdint.h>
// contains the cache handle needed for the struct rx_vm
#include <rx_cache.h>

// randomx virtual machine handle.
// Internals hidden inside rx.c
typedef struct rx_vm *hRXVM;

typedef char hash_t[32];

typedef enum {
    NO_ERR
} rx_res_t;

// Checks if the current system is compatible with the randomx miner
void compCheck();

// RandomX VM constructor. Returns a handle to the virtual machine.
// Arguments:
// None rn.
// Returns:
// Returns the handle to the randomx virtual machine.
hRXVM initVM(rx_cache_t *cache);

// Deallocates all of the heap memory the vm handle is pointing to and the heap memory used when allocating the handle itself.
// This means the vm handle doesn't need to freed and using after this will cause undefined behavior.
// Arguments:
// vm - The vm that will have its heap memory freed. The pointer itself will also be set to NULL afterwords. This argument must not be NULL.
void freeVM(hRXVM vm);

// Hashes the value using the VM.
// Arguments
// vm - The randomx vitual machine handle. This argument must not be NULL.
// out - The argument where the output of the hash is going to be written to.
// key - Optional key that can only be 60 bytes max.
// in - The value that will be hashed
rx_res_t rxHashVal(hRXVM vm, hash_t *out, char *key, char* in );
