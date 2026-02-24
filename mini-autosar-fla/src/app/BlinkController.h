/**
 * @file    BlinkController.h
 * @brief   Mini-AUTOSAR APP – BlinkController SWC
 * @version 1.0.0
 *
 * Responsibilities:
 *   - Reads button states from RTE.
 *   - Decides blink pattern:
 *       BTN0=off,  BTN1=off  → LED off
 *       BTN0=on,   BTN1=off  → Slow blink (1 Hz)
 *       BTN0=off,  BTN1=on   → Fast blink (5 Hz)
 *       BTN0=on,   BTN1=on   → Solid on
 *   - Drives blink state machine using system tick from GPT.
 *   - Writes LED levels and pattern to RTE.
 *
 * Runnable: BlinkController_Run() – period 10 ms
 */
#ifndef BLINK_CONTROLLER_H
#define BLINK_CONTROLLER_H

#include "../rte/Rte.h"

void BlinkController_Init(void);
void BlinkController_Run(void);

#endif /* BLINK_CONTROLLER_H */
