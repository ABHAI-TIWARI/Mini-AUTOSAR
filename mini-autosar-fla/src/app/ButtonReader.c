/**
 * @file    ButtonReader.c
 * @brief   Mini-AUTOSAR APP – ButtonReader SWC implementation
 */
#include "ButtonReader.h"
#include <stdio.h>

/* ------------------------------------------------------------------ */
/*  Debounce counters (private state)                                  */
/* ------------------------------------------------------------------ */
static uint8_t s_debounce0 = 0u;
static uint8_t s_debounce1 = 0u;
static ButtonState_t s_stableState0 = BTN_RELEASED;
static ButtonState_t s_stableState1 = BTN_RELEASED;

/* ------------------------------------------------------------------ */
/*  Helpers                                                            */
/* ------------------------------------------------------------------ */
static ButtonState_t debounce(ButtonState_t raw,
                               ButtonState_t stable,
                               uint8_t      *counter)
{
    if (raw == stable) {
        *counter = 0u;
    } else {
        (*counter)++;
        if (*counter >= BTN_DEBOUNCE_COUNTS) {
            stable   = raw;
            *counter = 0u;
        }
    }
    return stable;
}

/* ------------------------------------------------------------------ */
/*  SWC lifecycle                                                      */
/* ------------------------------------------------------------------ */
void ButtonReader_Init(void)
{
    s_debounce0   = 0u;
    s_debounce1   = 0u;
    s_stableState0 = BTN_RELEASED;
    s_stableState1 = BTN_RELEASED;
    printf("[APP ][ButtonReader] Init complete.\n");
}

void ButtonReader_Run(void)
{
    /* Read raw signals from RTE (which reads from BSW/Dio/MCAL) */
    ButtonState_t raw0 = Rte_Read_ButtonReader_Button0();
    ButtonState_t raw1 = Rte_Read_ButtonReader_Button1();

    /* Apply debounce */
    s_stableState0 = debounce(raw0, s_stableState0, &s_debounce0);
    s_stableState1 = debounce(raw1, s_stableState1, &s_debounce1);

    /* Write stable state back to RTE */
    Rte_Write_ButtonReader_Btn0State(s_stableState0);
    Rte_Write_ButtonReader_Btn1State(s_stableState1);
}
