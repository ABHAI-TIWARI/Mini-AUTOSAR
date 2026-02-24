/**
 * @file    Dio.h
 * @brief   Mini-AUTOSAR BSW – Digital I/O Service
 * @version 1.0.0
 * @ref     AUTOSAR_SWS_DIODriver (SWS-DIO-00003 etc.)
 *
 * The Dio module sits between the RTE/SWCs and MCAL.
 * It provides symbolic-channel access and enforces direction rules.
 */
#ifndef DIO_H
#define DIO_H

#include "Dio_Cfg.h"
#include "Det.h"

#define DIO_MODULE_ID   0x18u   /**< AUTOSAR Module ID for DIO */

typedef Mcal_Dio_ChannelType Dio_ChannelType;
typedef Mcal_Dio_LevelType   Dio_LevelType;

#define DIO_HIGH  MCAL_DIO_HIGH
#define DIO_LOW   MCAL_DIO_LOW

/* ------------------------------------------------------------------ */
/*  API                                                                */
/* ------------------------------------------------------------------ */
void          Dio_Init(void);
Dio_LevelType Dio_ReadChannel (Dio_ChannelType channelId);
void          Dio_WriteChannel(Dio_ChannelType channelId, Dio_LevelType level);

#endif /* DIO_H */
