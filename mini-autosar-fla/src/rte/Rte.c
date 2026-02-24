/**
 * @file    Rte.c
 * @brief   Mini-AUTOSAR RTE – Runtime Environment implementation
 *
 * Key design decisions:
 *  - All inter-SWC data lives in a single Rte_DataBuffer_t (copy-semantic).
 *  - Rte_Actuate() is called once per scheduler cycle to flush outputs
 *    to BSW/Dio.  This separates computation from I/O.
 *  - No dynamic allocation; everything is static.
 */
#include "Rte.h"
#include "../bsw/Dio.h"
#include "../bsw/Gpt.h"
#include <string.h>
#include <stdio.h>

/* ------------------------------------------------------------------ */
/*  Internal state                                                     */
/* ------------------------------------------------------------------ */
static Rte_DataBuffer_t s_buf;
static uint8_t          s_initialized = 0u;

/* ------------------------------------------------------------------ */
/*  Infrastructure                                                     */
/* ------------------------------------------------------------------ */
void Rte_Init(void)
{
    memset(&s_buf, 0, sizeof(s_buf));
    s_initialized = 1u;
    printf("[RTE ][Core] Init – signal buffer cleared.\n");
}

void Rte_Actuate(void)
{
    if (!s_initialized) return;

    /* Write LED levels computed by SWCs out to physical pins */
    Dio_WriteChannel(RTE_CFG_LED_STATUS_CHANNEL, s_buf.ledStatus);
    Dio_WriteChannel(RTE_CFG_LED_BLINK_CHANNEL,  s_buf.ledBlink);
    Dio_WriteChannel(RTE_CFG_LED_HB_CHANNEL,     s_buf.ledHeartbeat);
}

const Rte_DataBuffer_t* Rte_GetBuffer(void)
{
    return &s_buf;
}

/* ------------------------------------------------------------------ */
/*  ButtonReader ports                                                 */
/* ------------------------------------------------------------------ */
ButtonState_t Rte_Read_ButtonReader_Button0(void)
{
    return (Dio_ReadChannel(RTE_CFG_BTN0_CHANNEL) == DIO_HIGH)
           ? BTN_PRESSED : BTN_RELEASED;
}

ButtonState_t Rte_Read_ButtonReader_Button1(void)
{
    return (Dio_ReadChannel(RTE_CFG_BTN1_CHANNEL) == DIO_HIGH)
           ? BTN_PRESSED : BTN_RELEASED;
}

void Rte_Write_ButtonReader_Btn0State(ButtonState_t state)
{
    s_buf.btn0State = state;
}

void Rte_Write_ButtonReader_Btn1State(ButtonState_t state)
{
    s_buf.btn1State = state;
}

/* ------------------------------------------------------------------ */
/*  BlinkController ports                                              */
/* ------------------------------------------------------------------ */
ButtonState_t Rte_Read_BlinkCtrl_Btn0State(void)
{
    return s_buf.btn0State;
}

ButtonState_t Rte_Read_BlinkCtrl_Btn1State(void)
{
    return s_buf.btn1State;
}

void Rte_Write_BlinkCtrl_LedStatus(uint8_t level)
{
    s_buf.ledStatus = level;
}

void Rte_Write_BlinkCtrl_LedBlink(uint8_t level)
{
    s_buf.ledBlink = level;
}

void Rte_Write_BlinkCtrl_Pattern(BlinkPattern_t pattern)
{
    s_buf.blinkPattern = pattern;
}

/* ------------------------------------------------------------------ */
/*  Heartbeat ports                                                    */
/* ------------------------------------------------------------------ */
void Rte_Write_Heartbeat_LedHb(uint8_t level)
{
    s_buf.ledHeartbeat = level;
}
