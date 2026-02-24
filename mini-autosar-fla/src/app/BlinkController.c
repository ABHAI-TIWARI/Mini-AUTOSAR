/**
 * @file    BlinkController.c
 * @brief   Mini-AUTOSAR APP – BlinkController SWC implementation
 */
#include "BlinkController.h"
#include "../bsw/Gpt.h"
#include <stdio.h>

/* ------------------------------------------------------------------ */
/*  Blink period table (in ticks, 1 tick = 1 ms)                     */
/*  Half-period because we toggle each half cycle                     */
/* ------------------------------------------------------------------ */
#define SLOW_HALF_PERIOD_TICKS  500u   /* 1 Hz  → 1000ms → 500ms half */
#define FAST_HALF_PERIOD_TICKS  100u   /* 5 Hz  → 200ms  → 100ms half */

static BlinkPattern_t s_pattern      = BLINK_OFF;
static BlinkPattern_t s_lastPattern  = BLINK_OFF;
static Gpt_TickType   s_lastToggleTick = 0u;
static uint8_t        s_blinkState   = 0u;

static const char* patternName(BlinkPattern_t p)
{
    switch (p) {
        case BLINK_OFF:      return "OFF";
        case BLINK_SLOW:     return "SLOW (1Hz)";
        case BLINK_FAST:     return "FAST (5Hz)";
        case BLINK_SOLID_ON: return "SOLID ON";
        default:             return "UNKNOWN";
    }
}

void BlinkController_Init(void)
{
    s_pattern       = BLINK_OFF;
    s_lastPattern   = BLINK_OFF;
    s_lastToggleTick = 0u;
    s_blinkState    = 0u;
    printf("[APP ][BlinkCtrl]   Init complete.\n");
}

void BlinkController_Run(void)
{
    /* 1. Read inputs from RTE */
    ButtonState_t btn0 = Rte_Read_BlinkCtrl_Btn0State();
    ButtonState_t btn1 = Rte_Read_BlinkCtrl_Btn1State();

    /* 2. Decision logic (truth table) */
    if      (!btn0 && !btn1)  s_pattern = BLINK_OFF;
    else if ( btn0 && !btn1)  s_pattern = BLINK_SLOW;
    else if (!btn0 &&  btn1)  s_pattern = BLINK_FAST;
    else                       s_pattern = BLINK_SOLID_ON;

    /* Log only on pattern change */
    if (s_pattern != s_lastPattern) {
        printf("[APP ][BlinkCtrl]   Pattern changed → %s  "
               "(BTN0=%s BTN1=%s)\n",
               patternName(s_pattern),
               btn0 ? "PRESSED" : "released",
               btn1 ? "PRESSED" : "released");
        s_lastPattern    = s_pattern;
        s_lastToggleTick = Gpt_GetTick();
        s_blinkState     = 0u;
    }

    /* 3. Blink state machine */
    Gpt_TickType now      = Gpt_GetTick();
    Gpt_TickType elapsed  = now - s_lastToggleTick;
    uint8_t      ledLevel = 0u;

    switch (s_pattern) {
        case BLINK_OFF:
            ledLevel = 0u;
            break;

        case BLINK_SOLID_ON:
            ledLevel = 1u;
            break;

        case BLINK_SLOW:
            if (elapsed >= SLOW_HALF_PERIOD_TICKS) {
                s_blinkState     ^= 1u;
                s_lastToggleTick  = now;
            }
            ledLevel = s_blinkState;
            break;

        case BLINK_FAST:
            if (elapsed >= FAST_HALF_PERIOD_TICKS) {
                s_blinkState     ^= 1u;
                s_lastToggleTick  = now;
            }
            ledLevel = s_blinkState;
            break;
    }

    /* 4. Write outputs to RTE */
    Rte_Write_BlinkCtrl_LedBlink(ledLevel);
    Rte_Write_BlinkCtrl_LedStatus(btn0 | btn1);  /* status = any button active */
    Rte_Write_BlinkCtrl_Pattern(s_pattern);
}
