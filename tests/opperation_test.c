#include <opperations.h>
#include <stdint.h>
#include <test.h>
int testOppLE32() {
    u_int32_t input = 123456;
    // check if big endianess is correct
    T_ASSERT(input == 0x01E240)
    T_ASSERT(LE32(input) == 0x40E20100)
    return T_SUCCESS;
}

int testOppLE64() {
    u_int64_t input = 123456;
    // check if big endianess is correct
    T_ASSERT(input == 0x01E240)
    T_ASSERT((LE64(input) >> 32) == 0x40E20100)
    return T_SUCCESS;
}
