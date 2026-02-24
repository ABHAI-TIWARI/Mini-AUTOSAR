/**
 * @file    Mcal_Dio.c
 * @brief   Mini-AUTOSAR MCAL – Digital I/O implementation (mock)
 */
#include "Mcal_Dio.h"
#include <string.h>
#include <stdio.h>

/* ------------------------------------------------------------------ */
/*  Global virtual register bank                                       */
/* ------------------------------------------------------------------ */
Mcal_Dio_RegisterBank_t g_McalDioBank;

/* ------------------------------------------------------------------ */
/*  API implementation                                                 */
/* ------------------------------------------------------------------ */

void Mcal_Dio_Init(void)
{
    memset(&g_McalDioBank, 0, sizeof(g_McalDioBank));
    printf("[MCAL][Dio]  Init – %u virtual GPIO channels cleared.\n",
           MCAL_DIO_NUM_CHANNELS);
}

Mcal_Dio_LevelType Mcal_Dio_ReadChannel(Mcal_Dio_ChannelType channel)
{
    if (channel >= MCAL_DIO_NUM_CHANNELS) {
        return MCAL_DIO_LOW;   /* DET would fire in a full stack */
    }
    return (Mcal_Dio_LevelType)g_McalDioBank.pin_level[channel];
}

void Mcal_Dio_WriteChannel(Mcal_Dio_ChannelType channel,
                            Mcal_Dio_LevelType   level)
{
    if (channel >= MCAL_DIO_NUM_CHANNELS) {
        return;
    }
    /* Only write to channels configured as output */
    if (g_McalDioBank.pin_dir[channel] == 1u) {
        g_McalDioBank.pin_level[channel] = level;
    }
}

void Mcal_Dio_InjectInput(Mcal_Dio_ChannelType channel,
                           Mcal_Dio_LevelType   level)
{
    if (channel >= MCAL_DIO_NUM_CHANNELS) {
        return;
    }
    /* Force a pin level regardless of direction – for simulation only */
    g_McalDioBank.pin_level[channel] = level;
}
