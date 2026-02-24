/**
 * @file    Rte_Cfg.h
 * @brief   Mini-AUTOSAR RTE – Port/Signal configuration
 * @note    Auto-generated from config/ecu_config.yaml by scripts/gen_config.py
 *          DO NOT EDIT MANUALLY
 *
 * This header wires SWC port names to BSW Dio channels.
 * Changing a pin assignment = update YAML + re-run gen_config.py.
 * Application SWC code never changes.
 */
#ifndef RTE_CFG_H
#define RTE_CFG_H

#include "../bsw/Dio.h"
#include "../bsw/Gpt.h"

/* ------------------------------------------------------------------
 *  Signal → Physical channel mapping
 * ------------------------------------------------------------------ */
#define RTE_CFG_BTN0_CHANNEL        DIO_CHANNEL_BUTTON_0
#define RTE_CFG_BTN1_CHANNEL        DIO_CHANNEL_BUTTON_1
#define RTE_CFG_LED_STATUS_CHANNEL  DIO_CHANNEL_LED_STATUS
#define RTE_CFG_LED_BLINK_CHANNEL   DIO_CHANNEL_LED_BLINK
#define RTE_CFG_LED_HB_CHANNEL      DIO_CHANNEL_LED_HEARTBEAT

/* ------------------------------------------------------------------
 *  Scheduler timing (all in ticks, 1 tick = 1 ms)
 * ------------------------------------------------------------------ */
#define RTE_CFG_TASK_PERIOD_BUTTON_READER_MS    10u
#define RTE_CFG_TASK_PERIOD_BLINK_CTRL_MS       10u
#define RTE_CFG_TASK_PERIOD_HEARTBEAT_MS        500u

#endif /* RTE_CFG_H */
