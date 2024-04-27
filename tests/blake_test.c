#include <blake/blake.h>
#include <blake_generator.h>
#include <string.h>
#include <stdint.h>
#include <test.h>
// Test blake2b hashing with 64 byte output
int testBlake2bHashing64Byte() {
    unsigned char out[64];
    char* a = "cool";
    blake2bHash(a, strlen(a), 64, out);
    uint8_t expected_out[64] = { 0xf8, 0x93, 0x75, 0x9c, 0x78, 0xb5, 0xd9, 0xdb, 0x10, 0xc4, 0xa2, 0x04, 0x3e, 0x4c, 0x6b, 0xef, 0x2c, 0xc1, 0x3c, 0xf5, 0x75, 0x6c, 0x9c, 0x99, 0xbf, 0xb6, 0xec, 0x33, 0xd4, 0x25, 0x9d, 0xf3, 0x42, 0x10, 0x0f, 0x7c, 0x7e, 0x31, 0x3a, 0xfa, 0xe0, 0x9d, 0xe1, 0x06, 0x0a, 0x19, 0xce, 0xa6, 0x5d, 0x05, 0xb7, 0xa2, 0x91, 0x45, 0x60, 0x34, 0xf4, 0x8a, 0x2b, 0x15, 0x0e, 0xee, 0xa5, 0x5d };
    for (int i = 0; i < 64; ++i) {
        T_ASSERT(out[i] == expected_out[i]);
    }
    return T_SUCCESS;
}
// test hashing inputs longer than 128 characters. Because inputs longer that 128 chars go through special processing.
int testBlake2bHashingLargeInput() {
    unsigned char out[64];
    // long input
    char* a = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    blake2bHash(a, strlen(a), 64, out);
    uint8_t expected_out[64] = {0xff, 0x33, 0x1a, 0xc8, 0x66, 0x8e, 0x38, 0xcf, 0x5a, 0x48, 0x26, 0x55, 0x1a, 0x45, 0x13, 0x7e, 0x4e, 0xa9, 0x53, 0x0e, 0x4f, 0xd7, 0xf0, 0x70, 0xd7, 0xa0, 0x13, 0x97, 0xbd, 0xca, 0x17, 0x87, 0xb0, 0x7f, 0x77, 0x7c, 0x2d, 0x20, 0x2d, 0x20, 0xc7, 0x26, 0x07, 0x89, 0x76, 0xce, 0xd4, 0x67, 0x6d, 0xa0, 0x00, 0x7a, 0xd5, 0x83, 0x31, 0x2c, 0x4b, 0x1f, 0xb8, 0x65, 0xf9, 0x01, 0xcb, 0xbd };
    for (int i = 0; i < 64; ++i) {
        T_ASSERT(out[i] == expected_out[i]);
    }
    return T_SUCCESS;
}
// test outputs smaller that 64 characters
int testBlakeSmallOutput() {
    unsigned char out[12];
    // long input
    char* a = "hello";
    blake2bHash(a, strlen(a), 12, out);
    uint8_t expected_out[12] = { 0x8e, 0xd7, 0x25, 0x2f, 0x4b, 0x03, 0xf9, 0xe8, 0x37, 0x6d, 0xb5, 0xb2 };
    for (int i = 0; i < 12; ++i) {
        T_ASSERT(out[i] == expected_out[i]);
    }
    return T_SUCCESS;
}
// test if blake still works even when the input and output are the same variables
int testBlakeSameINandOUT() {
    unsigned char out[8] = {97, 97,97, 97,97, 97,97, 97};
    uint8_t expected_out[8] = {0xf3, 0x87, 0x02, 0x6a, 0x43, 0xb4, 0x37, 0x3c};
    blake2bHash((char*)out, 8, 8, out);
    for (int i = 0; i < 8; ++i) {
        T_ASSERT(out[i] == expected_out[i]);
    }
    return T_SUCCESS;
}
int testBlakeGenerator() {
    blake2_gen_t generator = initBlake2Gen("password", strlen("password"), 32);
    uint32_t expected_out[20] = { 183, 326339994, 140, 3176051710, 7, 2670490446, 216, 2493864758, 34, 1054331925, 105, 3281355903, 153, 580502502, 130, 1305963329, 217, 4110349275, 121, 2370955655 };
    uint8_t index = 0;
    while (index < 20) {
        uint8_t b = generateByte(&generator);
        uint32_t w = generateWord(&generator);
        FAILURE_LOG("i: %i b: %i w: %i\n", index, b, w);
        T_ASSERT(b == expected_out[index]);
        T_ASSERT(w == expected_out[++index]);
        ++index;
    }
    return T_SUCCESS;
}
