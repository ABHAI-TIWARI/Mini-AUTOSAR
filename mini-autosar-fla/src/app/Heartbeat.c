/**
 * @file    Heartbeat.c
 * @brief   Mini-AUTOSAR APP – Heartbeat SWC implementation
 */
#include "Heartbeat.h"
#include <stdio.h>

static uint8_t s_state = 0u;

void Heartbeat_Init(void)
{
    s_state = 0u;
    printf("[APP ][Heartbeat]   Init complete.\n");
}

void Heartbeat_Run(void)
{
    /* Toggle heartbeat LED */
    s_state ^= 1u;
    Rte_Write_Heartbeat_LedHb(s_state);
    printf("[APP ][Heartbeat]   ♥  tick → LED_HB=%s\n",
           s_state ? "ON " : "OFF");
}
