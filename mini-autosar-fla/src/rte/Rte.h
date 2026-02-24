/**
 * @file    Rte.h
 * @brief   Mini-AUTOSAR RTE – Runtime Environment API
 * @version 1.0.0
 * @ref     AUTOSAR_SWS_RTE
 *
 * The RTE provides the ONLY interface between SWCs and the rest of
 * the system.  SWCs are forbidden from calling BSW/MCAL directly.
 *
 * Naming convention: Rte_<Direction>_<PortName>_<SignalName>()
 *   Direction: Read | Write | Send | Receive | Call
 */
#ifndef RTE_H
#define RTE_H

#include "Rte_Types.h"
#include "Rte_Cfg.h"

/* ------------------------------------------------------------------ */
/*  RTE Read/Write API (port-based, SWC-facing)                       */
/* ------------------------------------------------------------------ */

/* --- ButtonReader SWC ports ---------------------------------------- */
/** Read physical button 0 state from BSW/MCAL */
ButtonState_t Rte_Read_ButtonReader_Button0(void);
/** Read physical button 1 state from BSW/MCAL */
ButtonState_t Rte_Read_ButtonReader_Button1(void);
/** Write button state into RTE buffer for BlinkController */
void          Rte_Write_ButtonReader_Btn0State(ButtonState_t state);
void          Rte_Write_ButtonReader_Btn1State(ButtonState_t state);

/* --- BlinkController SWC ports ------------------------------------ */
/** Read latest button states from RTE buffer */
ButtonState_t Rte_Read_BlinkCtrl_Btn0State(void);
ButtonState_t Rte_Read_BlinkCtrl_Btn1State(void);
/** Write LED commands to RTE buffer (actuated via BSW/Dio in Rte_Actuate) */
void          Rte_Write_BlinkCtrl_LedStatus(uint8_t level);
void          Rte_Write_BlinkCtrl_LedBlink (uint8_t level);
void          Rte_Write_BlinkCtrl_Pattern  (BlinkPattern_t pattern);

/* --- Heartbeat SWC ports ------------------------------------------ */
void          Rte_Write_Heartbeat_LedHb(uint8_t level);

/* ------------------------------------------------------------------ */
/*  RTE Infrastructure API                                             */
/* ------------------------------------------------------------------ */
void Rte_Init(void);
/** Flush RTE output buffers → BSW/Dio (called after each runnable set) */
void Rte_Actuate(void);
/** Expose internal buffer for unit testing */
const Rte_DataBuffer_t* Rte_GetBuffer(void);

#endif /* RTE_H */
