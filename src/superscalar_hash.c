#include "superscalar_hash.h"
#include <stdlib.h>
#include <string.h>
// Super scalar instructions. Specific instructions used to manipulate a given input.
typedef enum __attribute__((__packed__)) {
    EXE_PORT_NULL = 0,
    EXE_PORT_P0 = 1,
    EXE_PORT_P1 = 2,
    EXE_PORT_P5 = 4,
    EXE_PORT_P01 = 1 | 2,
    EXE_PORT_P05 = 1 | 4,
    EXE_PORT_P015 = 1 | 2 | 4,

} execution_port_t;
super_scalar_hash_t initSuperScalarHash(blake2_gen_t *gen) {
    ss_ins_t prog = malloc(sizeof(ss_ins_t) * SUPERSCALAR_MAX_SIZE);
    execution_port_t port_busy[3][CYCLE_MAP_SIZE];
    memset(&port_busy, 0, sizeof(execution_port_t) * 3 * CYCLE_MAP_SIZE);
}
