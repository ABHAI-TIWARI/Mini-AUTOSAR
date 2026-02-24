/**
 * @file    Gpt.h
 * @brief   Mini-AUTOSAR BSW – General Purpose Timer Service
 * @version 1.0.0
 */
#ifndef GPT_H
#define GPT_H

#include "../mcal/Mcal_Gpt.h"

#define GPT_MODULE_ID   0x74u

typedef Mcal_Gpt_TickType Gpt_TickType;

void         Gpt_Init(void);
Gpt_TickType Gpt_GetTick(void);
void         Gpt_Tick(void);   /**< Advance by one tick (called by scheduler) */

#endif /* GPT_H */
