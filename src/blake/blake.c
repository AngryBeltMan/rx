// Created by AngryBeltMan on github using these sources:
// https://en.wikipedia.org/wiki/BLAKE_(hash_function)
// https://datatracker.ietf.org/doc/html/rfc7693
#include <blake/blake.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
// Constant value for blake-256
#define ROUNDS_RUN 14
static const uint64_t IV_VECTOR[8] = {
    0x6a09e667f3bcc908,
    0xbb67ae8584caa73b,
    0x3c6ef372fe94f82b,
    0xa54ff53a5f1d36f1,
    0x510e527fade682d1,
    0x9b05688c2b3e6c1f,
    0x1f83d9abfb41bd6b,
    0x5be0cd19137e2179,
};
static const uint64_t SIGMA_TABLE[10][16] = {
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
    { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 },
    { 11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4 },
    {  7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8 },
    {  9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13 },
    {  2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9 },
    { 12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11 },
    { 13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10 },
    {  6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5 },
    { 10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13,  0 }
};

static void mix( uint64_t *a, uint64_t *b, uint64_t *c, uint64_t *d, uint64_t x, uint64_t y);
// Wrapper function for Blake512hash. Compresses the message.
// Arguments:
// final - Boolean value denoting if it is the final message block to be compressed. Info needed as all of the bits are inversed in the work vector if true.
static void compress(uint64_t (*state)[8], uint64_t msg_chunk[16], __uint128_t bytes_compressed, bool final) {
    uint64_t work_vec[16];
    memcpy(&work_vec,  state, sizeof(uint64_t) * 8);
    memcpy(&work_vec[8],  IV_VECTOR, sizeof(uint64_t) * 8);
    work_vec[12] = work_vec[12] ^ bytes_compressed;
    work_vec[13] = work_vec[13] ^ (bytes_compressed >> 64);
    if (final) {
        // invert all of the bits
        work_vec[14] ^= 0xFFFFFFFFFFFFFFFF;
    }
    for (char i = 0; i < 12; i++) {
        mix(&work_vec[0], &work_vec[4], &work_vec[8], &work_vec[12],  msg_chunk[SIGMA_TABLE[i % 10][0]],msg_chunk[SIGMA_TABLE[i % 10][1]]);
        mix(&work_vec[1], &work_vec[5], &work_vec[9], &work_vec[13],  msg_chunk[SIGMA_TABLE[i % 10][2]],msg_chunk[SIGMA_TABLE[i % 10][3]]);
        mix(&work_vec[2], &work_vec[6], &work_vec[10], &work_vec[14], msg_chunk[SIGMA_TABLE[i % 10][4]],msg_chunk[SIGMA_TABLE[i % 10][5]]);
        mix(&work_vec[3], &work_vec[7], &work_vec[11], &work_vec[15], msg_chunk[SIGMA_TABLE[i % 10][6]],msg_chunk[SIGMA_TABLE[i % 10][7]]);

        mix(&work_vec[0], &work_vec[5], &work_vec[10], &work_vec[15],msg_chunk[SIGMA_TABLE[i % 10][8]], msg_chunk[SIGMA_TABLE[i % 10][9]]);
        mix(&work_vec[1], &work_vec[6], &work_vec[11], &work_vec[12],msg_chunk[SIGMA_TABLE[i % 10][10]],msg_chunk[SIGMA_TABLE[i % 10][11]]);
        mix(&work_vec[2], &work_vec[7], &work_vec[8], &work_vec[13], msg_chunk[SIGMA_TABLE[i % 10][12]],msg_chunk[SIGMA_TABLE[i % 10][13]]);
        mix(&work_vec[3], &work_vec[4], &work_vec[9], &work_vec[14], msg_chunk[SIGMA_TABLE[i % 10][14]],msg_chunk[SIGMA_TABLE[i % 10][15]]);
    }
    for (int i = 0; i < 8; ++i) {
        (*state)[i] = (*state)[i] ^ work_vec[i] ^ work_vec[i + 8];
    }
}
// Hashes the message using blake2b.
// No heap memory is allocated.
// Arguments:
// message - The value that will be hashed. Cannot be NULL.
// mlen - The length of the message.
// outlen - The length of the output. Ranges from 1 to 64;
// out - Pointer value where the output will be written to. Cannot be NULL.
void blake2bHash(const char* message, size_t mlen, uint8_t outlen, unsigned char *out) {
    assert(outlen > 0 && outlen <= 64);
    hash_t h;
    memcpy(&h, IV_VECTOR, sizeof(uint64_t) * 8);
    h[0] = h[0] ^ 0x01010000 ^ outlen;

    // the amount of bytes compressed
    size_t compressed = 0;

    // process the chunk
    while (compressed + 128 < mlen) {
        uint64_t chunk[16];
        memcpy(&chunk, &message[compressed], 128);
        compressed += 128;
        compress(&h, chunk, compressed, false);
    }
    uint64_t final_chunk[16];
    // add padding
    memset(final_chunk, 0x0, 128);
    memcpy(&final_chunk, &message[compressed], mlen - compressed);
    compress(&h, final_chunk, mlen, true);
    // Change the output's endianess for big endian to little endian
    for (int i = 0; i < outlen; ++i) {
        out[i] = (h[i/8] & (uint64_t)0xFF << ((i % 8) * 8) ) >> ((i % 8) * 8);
    }
}


// rotates a 64 bit integer(_v) _s amount of bits to the right.
#define ROTATE_RIGHT(_v, _s) (((_v) >> _s) | ((_v) << (64 - _s)))
static void mix( uint64_t *a, uint64_t *b, uint64_t *c, uint64_t *d, uint64_t x, uint64_t y) {
    *a = (*a) + (*b) + x;
    *d = ROTATE_RIGHT((*d) ^ (*a), 32);

    *c = (*c) + (*d);
    *b = ROTATE_RIGHT((*b) ^ (*c), 24);

    *a = (*a) + (*b) + y;
    *d = ROTATE_RIGHT((*d) ^ (*a), 16);

    *c = (*c) + (*d);
    *b = ROTATE_RIGHT((*b) ^ (*c), 63);
}

