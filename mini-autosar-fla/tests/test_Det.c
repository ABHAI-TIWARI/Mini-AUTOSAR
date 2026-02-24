/**
 * @file    test_Det.c
 * @brief   Unit tests – Development Error Tracer
 */
#include "minitest.h"
#include "../src/bsw/Det.h"

int main(void)
{
    printf("\n--- test_Det ---\n");

    Det_Init();

    TEST("Error count starts at zero") {
        ASSERT_EQ(Det_GetErrorCount(), 0u);
    } END_TEST;

    TEST("Reporting an error increments count") {
        Det_ReportError(0x18u, 0u, 0x01u, DET_E_PARAM_INVALID);
        ASSERT_EQ(Det_GetErrorCount(), 1u);
    } END_TEST;

    TEST("Multiple errors accumulate") {
        Det_ReportError(0x74u, 0u, 0x01u, DET_E_UNINIT);
        Det_ReportError(0x18u, 0u, 0x02u, DET_E_BUFFER_OVERFLOW);
        ASSERT_EQ(Det_GetErrorCount(), 3u);
    } END_TEST;

    TEST("PrintLog runs without crash") {
        /* Just verify it does not segfault */
        Det_PrintLog();
        ASSERT_TRUE(1);
    } END_TEST;

    return TEST_RESULTS();
}
