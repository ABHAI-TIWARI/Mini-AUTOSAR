/**
 * @file    test_BlinkController.c
 * @brief   Unit tests – BlinkController SWC decision logic
 */
#include "minitest.h"
#include "../src/bsw/EcuM.h"
#include "../src/rte/Rte.h"
#include "../src/app/BlinkController.h"
#include "../src/mcal/Mcal_Dio.h"

static void setup(void)
{
    EcuM_Init();
    EcuM_StartupTwo();
    Rte_Init();
    BlinkController_Init();
}

static void setButtons(int btn0, int btn1)
{
    /* Write directly to RTE buffer via ButtonReader API mimicry */
    Rte_Write_ButtonReader_Btn0State(btn0 ? BTN_PRESSED : BTN_RELEASED);
    Rte_Write_ButtonReader_Btn1State(btn1 ? BTN_PRESSED : BTN_RELEASED);
}

int main(void)
{
    printf("\n--- test_BlinkController ---\n");

    setup();

    TEST("No buttons → BLINK_OFF") {
        setButtons(0, 0);
        BlinkController_Run();
        ASSERT_EQ(Rte_GetBuffer()->blinkPattern, BLINK_OFF);
        ASSERT_EQ(Rte_GetBuffer()->ledBlink, 0u);
    } END_TEST;

    TEST("BTN0 only → BLINK_SLOW") {
        setup();
        setButtons(1, 0);
        BlinkController_Run();
        ASSERT_EQ(Rte_GetBuffer()->blinkPattern, BLINK_SLOW);
    } END_TEST;

    TEST("BTN1 only → BLINK_FAST") {
        setup();
        setButtons(0, 1);
        BlinkController_Run();
        ASSERT_EQ(Rte_GetBuffer()->blinkPattern, BLINK_FAST);
    } END_TEST;

    TEST("Both buttons → BLINK_SOLID_ON") {
        setup();
        setButtons(1, 1);
        BlinkController_Run();
        ASSERT_EQ(Rte_GetBuffer()->blinkPattern, BLINK_SOLID_ON);
        ASSERT_EQ(Rte_GetBuffer()->ledBlink, 1u);
    } END_TEST;

    TEST("Status LED reflects any button active") {
        setup();
        setButtons(1, 0);
        BlinkController_Run();
        ASSERT_EQ(Rte_GetBuffer()->ledStatus, 1u);

        setButtons(0, 0);
        BlinkController_Run();
        ASSERT_EQ(Rte_GetBuffer()->ledStatus, 0u);
    } END_TEST;

    return TEST_RESULTS();
}
