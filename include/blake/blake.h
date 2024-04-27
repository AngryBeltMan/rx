// Created by AngryBeltMan on github using these sources:
// https://en.wikipedia.org/wiki/BLAKE_(hash_function)
// https://datatracker.ietf.org/doc/html/rfc7693
#pragma once
#include <stdlib.h>
#define OUTPUT_SIZE 8
#include <stdint.h>
typedef uint64_t hash_t[8];

void blake2bHash( const char* message, const size_t mlen, const uint8_t outlen, unsigned char *out);
