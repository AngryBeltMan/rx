#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// max chars that can be outputed during a test
#define MAX_OUT 20000
#ifdef TESTING
static size_t _failure_offset = 0;
static char* _failure_out = NULL;
#endif
// boolean value that checks if all of the tests passed
static int FAILED = 0;
static int PASSED = 0;
#define T_ASSERT(_cmp) if (!(_cmp)) {fprintf(stderr, "\033[38;2;255;25;0mAssertion Failed: (comparison: %s, function: %s, file: %s, line number: %i)\033[0m\n", #_cmp, __FUNCTION__, __FILE__, __LINE__); return T_FAILURE;}
#define T_SUCCESS 1
#define T_FAILURE 0

#define RUN_TEST(test_fn, ...) if (test_fn(__VA_ARGS__)) { \
    printf("\033[38;2;0;255;25mRESULT: FUNCTION %s PASSED\033[0m\n", #test_fn); \
    ++PASSED;\
} else { \
    LOG_FAILURE_INFO()\
    printf("\033[38;2;255;25;0mRESULT: FUNCTION %s FAILED\033[0m\n", #test_fn); ALL_PASSED = 0;\
    ++FAILED;\
} \
CLEAR_FAILURE_LOG();

#define RUN_TEST_NO_ARGS(test_fn) if (test_fn()) { \
    printf("\033[38;2;0;255;25mRESULT: FUNCTION %s PASSED\033[0m\n", #test_fn); \
    ++PASSED;\
} else { \
    LOG_FAILURE_INFO()\
    printf("\033[38;2;255;25;0mRESULT: FUNCTION %s FAILED\033[0m\n", #test_fn);\
    ++FAILED;\
} \
CLEAR_FAILURE_LOG()

#define PRINT_EVALUATION() \
    if (FAILED == 0) { \
        printf("\033[38;2;0;255;25mEVALUATION: %i out of %i tests passed\033[0m\n", PASSED, PASSED); \
    } else { \
        printf("\033[38;2;255;25;0mEVALUATION: %i out of %i tests passed\033[0m\n", PASSED, PASSED + FAILED );\
    }
#ifdef TESTING
// Initialize everything, should be called before testing
#define INIT_TESTING_ENV() _failure_out = calloc(MAX_OUT, 1); assert(_failure_out && "FAILED to failure output");
#define LOG_FAILURE_INFO() if (_failure_out != NULL) printf("--FAILURE LOGS:\n%s\n",_failure_out );
#define CLEAR_FAILURE_LOG() memset(_failure_out, 0, _failure_offset); _failure_offset = 0;
#define FAILURE_LOG(_fmt, ...)\
    snprintf(_failure_out + _failure_offset, MAX_OUT - _failure_offset, _fmt, __VA_ARGS__); \
    _failure_offset += strlen(_failure_out + _failure_offset);
#else
// if we aren't testing, calling these macros will do nothing
#define FAILURE_LOG(_fmt, ...)
#define LOG_FAILURE_INFO()
#define CLEAR_FAILURE_LOG()
#define INIT_TESTING_ENV()
#endif
