/**
 * @file    Mcal_Gpt.h
 * @brief   Mini-AUTOSAR MCAL – General Purpose Timer (mock, software counter)
 * @version 1.0.0
 * @ref     AUTOSAR_SWS_GPTDriver
 */
#ifndef MCAL_GPT_H
#define MCAL_GPT_H

#include <stdint.h>

typedef uint32_t Mcal_Gpt_TickType;   /**< Tick counter type */

/** Tick resolution label (informational) */
#define MCAL_GPT_TICK_US   1000u   /* 1 tick = 1 ms in this mock */

/* ------------------------------------------------------------------ */
/*  API                                                                */
/* ------------------------------------------------------------------ */
void             Mcal_Gpt_Init(void);
Mcal_Gpt_TickType Mcal_Gpt_GetTick(void);
void             Mcal_Gpt_IncrementTick(void);   /**< Called by scheduler each loop */

#endif /* MCAL_GPT_H */
