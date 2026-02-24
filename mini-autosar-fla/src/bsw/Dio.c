/**
 * @file    Dio.c
 * @brief   Mini-AUTOSAR BSW – Digital I/O Service implementation
 */
#include "Dio.h"
#include "../mcal/Mcal_Dio.h"
#include <stdio.h>

static uint8_t s_initialized = 0u;

void Dio_Init(void)
{
    /* Apply direction configuration from Dio_Cfg.h */
    static const uint8_t directions[] = DIO_CFG_DIRECTIONS;
    for (uint8_t i = 0u; i < MCAL_DIO_NUM_CHANNELS; i++) {
        g_McalDioBank.pin_dir[i] = directions[i];
    }
    s_initialized = 1u;
    printf("[BSW ][Dio]  Init – channel directions configured.\n");
}

Dio_LevelType Dio_ReadChannel(Dio_ChannelType channelId)
{
    if (!s_initialized) {
        Det_ReportError(DIO_MODULE_ID, 0u, 0x01u, DET_E_UNINIT);
        return DIO_LOW;
    }
    if (channelId >= MCAL_DIO_NUM_CHANNELS) {
        Det_ReportError(DIO_MODULE_ID, 0u, 0x01u, DET_E_PARAM_INVALID);
        return DIO_LOW;
    }
    return Mcal_Dio_ReadChannel(channelId);
}

void Dio_WriteChannel(Dio_ChannelType channelId, Dio_LevelType level)
{
    if (!s_initialized) {
        Det_ReportError(DIO_MODULE_ID, 0u, 0x02u, DET_E_UNINIT);
        return;
    }
    if (channelId >= MCAL_DIO_NUM_CHANNELS) {
        Det_ReportError(DIO_MODULE_ID, 0u, 0x02u, DET_E_PARAM_INVALID);
        return;
    }
    Mcal_Dio_WriteChannel(channelId, level);
}
