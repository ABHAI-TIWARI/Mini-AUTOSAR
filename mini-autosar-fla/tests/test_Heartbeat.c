/**
 * @file    test_Heartbeat.c
 * @brief   Unit tests – Heartbeat SWC
 */
#include "minitest.h"
#include "../src/bsw/EcuM.h"
#include "../src/rte/Rte.h"
#include "../src/app/Heartbeat.h"

int main(void)
{
    printf("\n--- test_Heartbeat ---\n");

    EcuM_Init();
    EcuM_StartupTwo();
    Rte_Init();
    Heartbeat_Init();

    TEST("First Run sets HB LED ON") {
        Heartbeat_Run();
        ASSERT_EQ(Rte_GetBuffer()->ledHeartbeat, 1u);
    } END_TEST;

    TEST("Second Run toggles HB LED OFF") {
        Heartbeat_Run();
        ASSERT_EQ(Rte_GetBuffer()->ledHeartbeat, 0u);
    } END_TEST;

    TEST("Four Runs returns to original state (two full cycles)") {
        uint8_t start = Rte_GetBuffer()->ledHeartbeat;
        Heartbeat_Run();
        Heartbeat_Run();
        ASSERT_EQ(Rte_GetBuffer()->ledHeartbeat, start);
    } END_TEST;

    return TEST_RESULTS();
}
