/**
 * @file    minitest.h
 * @brief   Minimal unit test framework – no external dependencies
 *
 * Usage:
 *   TEST("description") { ASSERT_EQ(a, b); }
 *   return TEST_RESULTS();
 */
#ifndef MINITEST_H
#define MINITEST_H

#include <stdio.h>
#include <string.h>

static int _mt_pass = 0;
static int _mt_fail = 0;

#define TEST(desc) \
    do { \
        const char *_mt_desc = (desc); \
        int _mt_ok = 1;

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("  FAIL [%s] line %d: expected %d got %d\n", \
                   _mt_desc, __LINE__, (int)(b), (int)(a)); \
            _mt_ok = 0; \
        } \
    } while(0)

#define ASSERT_NE(a, b) \
    do { \
        if ((a) == (b)) { \
            printf("  FAIL [%s] line %d: expected != %d\n", \
                   _mt_desc, __LINE__, (int)(b)); \
            _mt_ok = 0; \
        } \
    } while(0)

#define ASSERT_TRUE(x) ASSERT_EQ((x) ? 1 : 0, 1)
#define ASSERT_FALSE(x) ASSERT_EQ((x) ? 1 : 0, 0)

#define END_TEST \
        if (_mt_ok) { printf("  PASS [%s]\n", _mt_desc); _mt_pass++; } \
        else { _mt_fail++; } \
    } while(0)

#define TEST_RESULTS() \
    ( printf("\n=== Results: %d passed, %d failed ===\n", \
             _mt_pass, _mt_fail), \
      (_mt_fail > 0) ? 1 : 0 )

#endif /* MINITEST_H */
