/**
 * @file    Mcal_Gpt.c
 * @brief   Mini-AUTOSAR MCAL – GPT implementation (mock software counter)
 */
#include "Mcal_Gpt.h"
#include <stdio.h>

static Mcal_Gpt_TickType s_tick = 0u;

void Mcal_Gpt_Init(void)
{
    s_tick = 0u;
    printf("[MCAL][Gpt]  Init – software tick counter reset.\n");
}

Mcal_Gpt_TickType Mcal_Gpt_GetTick(void)
{
    return s_tick;
}

void Mcal_Gpt_IncrementTick(void)
{
    s_tick++;
}
