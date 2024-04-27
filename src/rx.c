#include "rx_cache.h"
#include <rx.h>
#include <float.h>
#include <assert.h>
#include <stdlib.h>

struct rx_vm {
};

void compCheck() {
    // checks to see if the floating point values of the system follow the IEEE 754 convention
    assert(DBL_MANT_DIG == 53);
    assert(DBL_MIN_EXP == -1021);
    assert(DBL_MAX_EXP == 1024);
}

hRXVM initVM(rx_cache_t *cache) {
    assert(cache && "Cache is NULL!");
    struct rx_vm *vm = malloc(sizeof(struct rx_vm));
    return vm;
}

rx_res_t rxHashVal(hRXVM vm, hash_t *out, char *key, char *in) {
    return 0;
}
