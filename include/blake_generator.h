// Blake2 generator. Used to generate psuedo-random 1 or 4 byte numbers
#include <stdint.h>
#define BLAKE_GEN_DATA_LEN 64
// Blake generator
typedef struct {
    uint8_t index;
    uint8_t data[BLAKE_GEN_DATA_LEN];
} blake2_gen_t;
// Generate psuedo-random bytes using a given blake2 generator.
// Arguments:
// gen - Pointer to the blake generator. Used to generate the psuedo-random bytes.
uint8_t generateByte(blake2_gen_t *gen);

// Generate psuedo-random 4 byte word using a given blake2 generator.
// gen - Pointer to the blake generator. Used to generate the psuedo-random bytes.
uint32_t generateWord(blake2_gen_t *gen);

// Construct the blake2 generator using a key.
// This is done by concatenating the key, padding with zeros to make it 60 bytes long, and the 4 byte nonce.
// This 64 byte value is then hashed using the blake2d hash.
// Arguments:
// key - A char array that will be used as an inital seed value. Pointer cannot be null.
// key_len - The length of the argument key. Value must be between 0 to 60.
// nonce - Additional 4 bytes of data that will be appended to the seed.
blake2_gen_t initBlake2Gen(char* key, uint8_t key_len, uint32_t nonce);
