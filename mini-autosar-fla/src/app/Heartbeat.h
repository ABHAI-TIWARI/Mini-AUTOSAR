/**
 * @file    Heartbeat.h
 * @brief   Mini-AUTOSAR APP – Heartbeat SWC
 * @version 1.0.0
 *
 * Responsibilities:
 *   - Toggles a dedicated heartbeat LED every 500 ms (configurable).
 *   - Provides a visible "ECU is alive" signal independent of other SWCs.
 *
 * Runnable: Heartbeat_Run() – period 500 ms
 */
#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "../rte/Rte.h"

void Heartbeat_Init(void);
void Heartbeat_Run(void);

#endif /* HEARTBEAT_H */
