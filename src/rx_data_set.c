#include <rx_data_set.h>
#include <rx_cache.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static rx_ds_item_t initItem(char* key, uint8_t key_len) {
    rx_ds_item_t item;
    memset(&item.r, 0, 64);
    memcpy(&item.r, key, key_len);
}
rx_dataset_t initDataSet(rx_cache_t *p_cache) {
    rx_dataset_t dataset;
    dataset.items = malloc(RANDOMX_DATASET_EXTRA_SIZE + RANDOMX_DATASET_BASE_SIZE);
    for (uint32_t item_num = 0; item_num < (RANDOMX_DATASET_BASE_SIZE + RANDOMX_DATASET_EXTRA_SIZE) / 64; ++item_num) {

    }
    return dataset;
}

void freeDataSet(rx_dataset_t *p_dataset) {
    free(p_dataset->items);
}
