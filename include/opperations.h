// Written by AngryBeltMan on github.com
// Contains important opperations for argon2d.
#pragma once
// header file that contains opperations need to do the cyptographic hashing

#include <sys/types.h>
// rotates a 64 bit number left _s many places
#define ROTATE_LEFT(_n, _s) (_n << _s) ^ (_n >> (64 - _s))

// rotates a 64 bit number left _s many places
#define ROTATE_RIGHT(_n, _s) (_n >> _s) ^ (_n << (64 - _s))
// converts the bytes into little endian order not the bits
#define LE32(_a) (((_a & 0xFF)<<24) + ((_a & 0xFF00) << 8) + ((_a & 0xFF0000)>>8)+ ((_a & 0xFF000000)>>24))
// converts a 64 bit number into little endian order
#define LE64(_a) (((_a & 0xFF)<<56) + ((_a & 0xFF00) << 40) + ((_a & 0xFF0000)<<24)+ ((_a & 0xFF000000) << 8) + ((_a & 0xFF00000000)>>8)+ ((_a & 0xFF0000000000)>>24))
// truncates a 64 bit number to only having the first 32 least significant bits
#define TRUNC(_a) (_a & 0xFFFFFFFF)

// xor two memory blocks. Memory blocks can be found in argon2_mem.h.
// Arguments:
// _store - The memory block that will hold the result of left and right block being xored
// _lblock - Left hand side of the xor opperation.
// _rblock - Right hand side of the xor opperation.
#define XOR_BLOCK(_store, _lblock, _rblock) do {\
    _store.reg[0]  = _lblock.reg[0]  ^ _rblock.reg[0];\
    _store.reg[1]  = _lblock.reg[1]  ^ _rblock.reg[1];\
    _store.reg[2]  = _lblock.reg[2]  ^ _rblock.reg[2];\
    _store.reg[3]  = _lblock.reg[3]  ^ _rblock.reg[3];\
    _store.reg[4]  = _lblock.reg[4]  ^ _rblock.reg[4];\
    _store.reg[5]  = _lblock.reg[5]  ^ _rblock.reg[5];\
    _store.reg[6]  = _lblock.reg[6]  ^ _rblock.reg[6];\
    _store.reg[7]  = _lblock.reg[7]  ^ _rblock.reg[7];\
    _store.reg[8]  = _lblock.reg[8]  ^ _rblock.reg[8];\
    _store.reg[9]  = _lblock.reg[9]  ^ _rblock.reg[9];\
    _store.reg[10] = _lblock.reg[10] ^ _rblock.reg[10];\
    _store.reg[11] = _lblock.reg[11] ^ _rblock.reg[11];\
    _store.reg[12] = _lblock.reg[12] ^ _rblock.reg[12];\
    _store.reg[13] = _lblock.reg[13] ^ _rblock.reg[13];\
    _store.reg[14] = _lblock.reg[14] ^ _rblock.reg[14];\
    _store.reg[15] = _lblock.reg[15] ^ _rblock.reg[15];\
    _store.reg[16] = _lblock.reg[16] ^ _rblock.reg[16];\
    _store.reg[17] = _lblock.reg[17] ^ _rblock.reg[17];\
    _store.reg[18] = _lblock.reg[18] ^ _rblock.reg[18];\
    _store.reg[19] = _lblock.reg[19] ^ _rblock.reg[19];\
    _store.reg[20] = _lblock.reg[20] ^ _rblock.reg[20];\
    _store.reg[21] = _lblock.reg[21] ^ _rblock.reg[21];\
    _store.reg[22] = _lblock.reg[22] ^ _rblock.reg[22];\
    _store.reg[23] = _lblock.reg[23] ^ _rblock.reg[23];\
    _store.reg[24] = _lblock.reg[24] ^ _rblock.reg[24];\
    _store.reg[25] = _lblock.reg[25] ^ _rblock.reg[25];\
    _store.reg[26] = _lblock.reg[26] ^ _rblock.reg[26];\
    _store.reg[27] = _lblock.reg[27] ^ _rblock.reg[27];\
    _store.reg[28] = _lblock.reg[28] ^ _rblock.reg[28];\
    _store.reg[29] = _lblock.reg[29] ^ _rblock.reg[29];\
    _store.reg[30] = _lblock.reg[30] ^ _rblock.reg[30];\
    _store.reg[31] = _lblock.reg[31] ^ _rblock.reg[31];\
    _store.reg[32] = _lblock.reg[32] ^ _rblock.reg[32];\
    _store.reg[33] = _lblock.reg[33] ^ _rblock.reg[33];\
    _store.reg[34] = _lblock.reg[34] ^ _rblock.reg[34];\
    _store.reg[35] = _lblock.reg[35] ^ _rblock.reg[35];\
    _store.reg[36] = _lblock.reg[36] ^ _rblock.reg[36];\
    _store.reg[37] = _lblock.reg[37] ^ _rblock.reg[37];\
    _store.reg[38] = _lblock.reg[38] ^ _rblock.reg[38];\
    _store.reg[39] = _lblock.reg[39] ^ _rblock.reg[39];\
    _store.reg[40] = _lblock.reg[40] ^ _rblock.reg[40];\
    _store.reg[41] = _lblock.reg[41] ^ _rblock.reg[41];\
    _store.reg[42] = _lblock.reg[42] ^ _rblock.reg[42];\
    _store.reg[43] = _lblock.reg[43] ^ _rblock.reg[43];\
    _store.reg[44] = _lblock.reg[44] ^ _rblock.reg[44];\
    _store.reg[45] = _lblock.reg[45] ^ _rblock.reg[45];\
    _store.reg[46] = _lblock.reg[46] ^ _rblock.reg[46];\
    _store.reg[47] = _lblock.reg[47] ^ _rblock.reg[47];\
    _store.reg[48] = _lblock.reg[48] ^ _rblock.reg[48];\
    _store.reg[49] = _lblock.reg[49] ^ _rblock.reg[49];\
    _store.reg[50] = _lblock.reg[50] ^ _rblock.reg[50];\
    _store.reg[51] = _lblock.reg[51] ^ _rblock.reg[51];\
    _store.reg[52] = _lblock.reg[52] ^ _rblock.reg[52];\
    _store.reg[53] = _lblock.reg[53] ^ _rblock.reg[53];\
    _store.reg[54] = _lblock.reg[54] ^ _rblock.reg[54];\
    _store.reg[55] = _lblock.reg[55] ^ _rblock.reg[55];\
    _store.reg[56] = _lblock.reg[56] ^ _rblock.reg[56];\
    _store.reg[57] = _lblock.reg[57] ^ _rblock.reg[57];\
    _store.reg[58] = _lblock.reg[58] ^ _rblock.reg[58];\
    _store.reg[59] = _lblock.reg[59] ^ _rblock.reg[59];\
    _store.reg[60] = _lblock.reg[60] ^ _rblock.reg[60];\
    _store.reg[61] = _lblock.reg[61] ^ _rblock.reg[61];\
    _store.reg[62] = _lblock.reg[62] ^ _rblock.reg[62];\
    _store.reg[63] = _lblock.reg[63] ^ _rblock.reg[63];\
} while (0);
