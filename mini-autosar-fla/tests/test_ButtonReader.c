/**
 * @file    test_ButtonReader.c
 * @brief   Unit tests – ButtonReader SWC (debounce logic)
 */
#include "minitest.h"
#include "../src/bsw/EcuM.h"
#include "../src/rte/Rte.h"
#include "../src/app/ButtonReader.h"
#include "../src/mcal/Mcal_Dio.h"
#include "../src/bsw/Gpt.h"

static void setup(void)
{
    EcuM_Init();
    EcuM_StartupTwo();
    Rte_Init();
    ButtonReader_Init();
}

int main(void)
{
    printf("\n--- test_ButtonReader ---\n");

    setup();

    TEST("Button released with no input") {
        /* Channel 0 = BTN0, injected LOW */
        Mcal_Dio_InjectInput(0u, MCAL_DIO_LOW);
        ButtonReader_Run();
        ASSERT_EQ(Rte_GetBuffer()->btn0State, BTN_RELEASED);
    } END_TEST;

    TEST("Button press detected after debounce threshold") {
        Mcal_Dio_InjectInput(0u, MCAL_DIO_HIGH);
        /* Must call Run BTN_DEBOUNCE_COUNTS times to confirm */
        for (int i = 0; i < BTN_DEBOUNCE_COUNTS; i++) {
            ButtonReader_Run();
        }
        ASSERT_EQ(Rte_GetBuffer()->btn0State, BTN_PRESSED);
    } END_TEST;

    TEST("Single noisy sample does not flip stable state") {
        /* Button was pressed (from previous test), inject one LOW glitch */
        setup();
        /* Prime state to PRESSED */
        Mcal_Dio_InjectInput(0u, MCAL_DIO_HIGH);
        for (int i = 0; i < BTN_DEBOUNCE_COUNTS; i++) ButtonReader_Run();
        ASSERT_EQ(Rte_GetBuffer()->btn0State, BTN_PRESSED);

        /* One glitch LOW – should NOT flip yet */
        Mcal_Dio_InjectInput(0u, MCAL_DIO_LOW);
        ButtonReader_Run();
        ASSERT_EQ(Rte_GetBuffer()->btn0State, BTN_PRESSED);  /* still PRESSED */
    } END_TEST;

    TEST("BTN1 independently tracked") {
        setup();
        Mcal_Dio_InjectInput(1u, MCAL_DIO_HIGH);
        for (int i = 0; i < BTN_DEBOUNCE_COUNTS; i++) ButtonReader_Run();
        ASSERT_EQ(Rte_GetBuffer()->btn1State, BTN_PRESSED);
        ASSERT_EQ(Rte_GetBuffer()->btn0State, BTN_RELEASED);
    } END_TEST;

    return TEST_RESULTS();
}
