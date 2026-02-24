/**
 * @file    ButtonReader.h
 * @brief   Mini-AUTOSAR APP – ButtonReader SWC
 * @version 1.0.0
 *
 * Responsibilities:
 *   - Reads two physical buttons via RTE.
 *   - Applies debounce logic (N consecutive same samples = stable).
 *   - Writes stable button state back to RTE for BlinkController.
 *
 * Runnable: ButtonReader_Run()  – period 10 ms
 */
#ifndef BUTTON_READER_H
#define BUTTON_READER_H

#include "../rte/Rte.h"

#define BTN_DEBOUNCE_COUNTS  3u   /**< Consecutive samples to confirm state */

void ButtonReader_Init(void);
void ButtonReader_Run(void);

#endif /* BUTTON_READER_H */
