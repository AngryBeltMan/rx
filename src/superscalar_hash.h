// Written by AngryBeltMan on github.
// References:
// https://github.com/Ragnaroek/mithril/blob/master/src/randomx/superscalar.rs#L12
#pragma once
#include <blake_generator.h>
#define SUPERSCALAR_MAX_SIZE 3 * RANDOMX_SUPERSCALAR_LATENCY + 2
#define RANDOMX_SUPERSCALAR_LATENCY 170
#define CYCLE_MAP_SIZE RANDOMX_SUPERSCALAR_LATENCY + 4
// Super scalar instructions. Specific instructions used to manipulate a given input.
typedef enum __attribute__((__packed__)) {
    INS_ADD
} ss_ins_t;

typedef struct {
    // instructions the superscalar hash function will run when called
    ss_ins_t *ins;
} super_scalar_hash_t;
// initalize a singluar super scalar hash function
// Arguments:
// gen - Blake2 generator used for the creation of the superscalar hash functions. Construction of the generator can be found in blake_generator.h.
super_scalar_hash_t initSuperScalarHash(blake2_gen_t *gen);
