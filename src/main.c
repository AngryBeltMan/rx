#include "rx_cache.h"
#include <argon2d.h>
#include <argon_core.h>
#include <argon2_mem.h>
#include <string.h>
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


int main() {
    initCache("password", strlen("password"));
}
