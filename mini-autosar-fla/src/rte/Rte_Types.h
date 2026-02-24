/**
 * @file    Rte_Types.h
 * @brief   Mini-AUTOSAR RTE – Shared data types and inter-SWC signals
 */
#ifndef RTE_TYPES_H
#define RTE_TYPES_H

#include <stdint.h>

/* ------------------------------------------------------------------
 *  Application-level signal types
 * ------------------------------------------------------------------ */

/** Button state as seen by the application */
typedef enum {
    BTN_RELEASED = 0,
    BTN_PRESSED  = 1
} ButtonState_t;

/** Blink pattern commanded by BlinkController */
typedef enum {
    BLINK_OFF      = 0,   /**< LED always off        */
    BLINK_SLOW     = 1,   /**< 1 Hz blink            */
    BLINK_FAST     = 2,   /**< 5 Hz blink            */
    BLINK_SOLID_ON = 3    /**< LED always on         */
} BlinkPattern_t;

/* ------------------------------------------------------------------
 *  Inter-SWC shared signals (RTE internal buffers)
 * ------------------------------------------------------------------ */
typedef struct {
    ButtonState_t  btn0State;         /**< Written by ButtonReader       */
    ButtonState_t  btn1State;         /**< Written by ButtonReader       */
    BlinkPattern_t blinkPattern;      /**< Written by BlinkController    */
    uint8_t        ledStatus;         /**< Written by BlinkController    */
    uint8_t        ledBlink;          /**< Written by BlinkController    */
    uint8_t        ledHeartbeat;      /**< Written by Heartbeat          */
} Rte_DataBuffer_t;

#endif /* RTE_TYPES_H */
