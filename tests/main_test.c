#include <stdio.h>
#include "blake_test.c"
#include <test.h>
#include "opperation_test.c"
#include <argon2d.h>
int main() {
    INIT_TESTING_ENV();
    RUN_TEST_NO_ARGS(testBlakeGenerator);
    PRINT_EVALUATION();
}
