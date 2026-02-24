/**
 * @file    EcuM.h
 * @brief   Mini-AUTOSAR BSW – ECU Manager (simplified)
 * @version 1.0.0
 * @ref     AUTOSAR_SWS_ECUStateManager
 *
 * EcuM orchestrates the startup sequence:
 *   EcuM_Init → MCAL_Init → BSW_Init → RTE_Start
 */
#ifndef ECUM_H
#define ECUM_H

typedef enum {
    ECUM_STATE_OFF       = 0,
    ECUM_STATE_STARTUP   = 1,
    ECUM_STATE_RUN       = 2,
    ECUM_STATE_SHUTDOWN  = 3
} EcuM_StateType;

void         EcuM_Init(void);
void         EcuM_StartupTwo(void);   /**< BSW + RTE init phase */
EcuM_StateType EcuM_GetState(void);
void         EcuM_Shutdown(void);

#endif /* ECUM_H */
