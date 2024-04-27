#pragma once
#include "rx_cache.h"
#include <stdint.h>
#define RANDOMX_DATASET_BASE_SIZE 2147483648
#define RANDOMX_DATASET_EXTRA_SIZE 33554368
// Randomx Dataset Item. An array of these are used to create the dataset.
// See: https://github.com/tevador/RandomX/blob/master/doc/specs.md#7-dataset
// Struct should be packed because there is only one attribute. However packed is specified just to be sure.
typedef struct __attribute__((__packed__)) {
    // 7.3 "The item data is represented by 8 64-bit integer registers: r0-r7"
    uint64_t r[8];
} rx_ds_item_t;
// RandomX Dataset.
typedef struct rx_dataset {
    rx_ds_item_t *items;
} rx_dataset_t;

// Create the dataset using blocks inside the created cache as reference.
rx_dataset_t initDataSet(rx_cache_t *p_cache);

// frees all of the heap memory that lies inside the dataset
void freeDataSet(rx_dataset_t *p_dataset);
