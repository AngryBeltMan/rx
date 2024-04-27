#include <assert.h>
#include <blake_generator.h>
#include <blake/blake.h>
#include <stdint.h>
#include <string.h>
// Reinitalize the blake2Generator when all of the bytes have been exhausted. This is done by using the previous hash as input to generate a new hash.
// If all of the bytes haven't been exhausted, nothing is modified and no hashing is done.
static void reinitBlake2Gen(blake2_gen_t *generator, uint8_t bytes_generated) {
    if (generator->index + bytes_generated > BLAKE_GEN_DATA_LEN) {
        blake2bHash((char*)generator->data, 64, 64, generator->data);
        generator->index = 0;
    }
}
uint8_t generateByte(blake2_gen_t *gen) {
    // check to see if the generator needs to be reinitalized
    reinitBlake2Gen(gen, 1);
    uint8_t out;
    memcpy(&out, &gen->data[gen->index], 1);
    gen->index += 1;
    return out;
}

uint32_t generateWord(blake2_gen_t *gen) {
    // check to see if the generator needs to be reinitalized
    reinitBlake2Gen(gen, 4);
    uint32_t out;
    memcpy(&out, &gen->data[gen->index], 4);
    gen->index += 4;
    return out;
}

blake2_gen_t initBlake2Gen(char *key, uint8_t key_len, uint32_t nonce) {
    blake2_gen_t generator;
    generator.index = 0;
    memcpy(&generator.data, key, key_len);
    // pad the remaining 60 bytes of the key with zeros
    memset(&generator.data[key_len], 0x0, 60 - key_len );
    memcpy(&generator.data[60], &nonce, 4);
    blake2bHash((char*)generator.data, 64, 64, generator.data);
    return generator;
}
