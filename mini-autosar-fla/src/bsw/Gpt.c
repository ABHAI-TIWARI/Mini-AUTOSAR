/**
 * @file    Gpt.c
 * @brief   Mini-AUTOSAR BSW – GPT Service implementation
 */
#include "Gpt.h"
#include <stdio.h>

void Gpt_Init(void)
{
    Mcal_Gpt_Init();
    printf("[BSW ][Gpt]  Init – tick service ready.\n");
}

Gpt_TickType Gpt_GetTick(void)
{
    return Mcal_Gpt_GetTick();
}

void Gpt_Tick(void)
{
    Mcal_Gpt_IncrementTick();
}
