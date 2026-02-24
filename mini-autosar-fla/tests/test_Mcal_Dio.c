/**
 * @file    test_Mcal_Dio.c
 * @brief   Unit tests – MCAL Dio virtual GPIO
 */
#include "minitest.h"
#include "../src/mcal/Mcal_Dio.h"

int main(void)
{
    printf("\n--- test_Mcal_Dio ---\n");

    Mcal_Dio_Init();

    TEST("Init clears all channels to LOW") {
        for (int i = 0; i < MCAL_DIO_NUM_CHANNELS; i++) {
            ASSERT_EQ(g_McalDioBank.pin_level[i], 0u);
        }
    } END_TEST;

    TEST("WriteChannel to output pin sets level HIGH") {
        g_McalDioBank.pin_dir[2] = 1u;   /* channel 2 = output */
        Mcal_Dio_WriteChannel(2u, MCAL_DIO_HIGH);
        ASSERT_EQ(Mcal_Dio_ReadChannel(2u), MCAL_DIO_HIGH);
    } END_TEST;

    TEST("WriteChannel to input pin is ignored") {
        g_McalDioBank.pin_dir[0] = 0u;   /* channel 0 = input */
        Mcal_Dio_WriteChannel(0u, MCAL_DIO_HIGH);
        ASSERT_EQ(Mcal_Dio_ReadChannel(0u), MCAL_DIO_LOW);  /* not changed */
    } END_TEST;

    TEST("InjectInput overrides direction restriction") {
        g_McalDioBank.pin_dir[0] = 0u;
        Mcal_Dio_InjectInput(0u, MCAL_DIO_HIGH);
        ASSERT_EQ(Mcal_Dio_ReadChannel(0u), MCAL_DIO_HIGH);
    } END_TEST;

    TEST("Out-of-range channel read returns LOW") {
        ASSERT_EQ(Mcal_Dio_ReadChannel(0xFFu), MCAL_DIO_LOW);
    } END_TEST;

    return TEST_RESULTS();
}
