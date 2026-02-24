/**
 * @file    Mcal_Dio.h
 * @brief   Mini-AUTOSAR MCAL – Digital I/O (mock, in-memory GPIO)
 * @version 1.0.0
 * @ref     AUTOSAR_SWS_DIODriver
 *
 * This module simulates a physical GPIO register bank using a plain
 * C struct.  No real hardware is touched; it runs on any host PC.
 */
#ifndef MCAL_DIO_H
#define MCAL_DIO_H

#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Type definitions (mirror AUTOSAR naming)                           */
/* ------------------------------------------------------------------ */
typedef uint8_t  Mcal_Dio_ChannelType;   /**< GPIO channel index      */
typedef uint8_t  Mcal_Dio_LevelType;     /**< STD_HIGH=1 / STD_LOW=0  */

#define MCAL_DIO_HIGH  ((Mcal_Dio_LevelType)1u)
#define MCAL_DIO_LOW   ((Mcal_Dio_LevelType)0u)

/** Number of virtual GPIO channels in this mock ECU */
#define MCAL_DIO_NUM_CHANNELS  8u

/* ------------------------------------------------------------------ */
/*  Virtual GPIO register bank                                         */
/* ------------------------------------------------------------------ */
typedef struct {
    uint8_t pin_level[MCAL_DIO_NUM_CHANNELS]; /**< Current output/input level */
    uint8_t pin_dir[MCAL_DIO_NUM_CHANNELS];   /**< 0=input, 1=output          */
} Mcal_Dio_RegisterBank_t;

/** Singleton "hardware" register bank (defined in Mcal_Dio.c) */
extern Mcal_Dio_RegisterBank_t g_McalDioBank;

/* ------------------------------------------------------------------ */
/*  API                                                                 */
/* ------------------------------------------------------------------ */
void             Mcal_Dio_Init(void);
Mcal_Dio_LevelType Mcal_Dio_ReadChannel (Mcal_Dio_ChannelType channel);
void             Mcal_Dio_WriteChannel(Mcal_Dio_ChannelType channel,
                                        Mcal_Dio_LevelType   level);
/** Test helper: inject an input level (simulates physical button press) */
void             Mcal_Dio_InjectInput(Mcal_Dio_ChannelType channel,
                                      Mcal_Dio_LevelType   level);

#endif /* MCAL_DIO_H */
