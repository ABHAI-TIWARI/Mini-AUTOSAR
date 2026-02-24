/**
 * @file    EcuM.c
 * @brief   Mini-AUTOSAR BSW – ECU Manager implementation
 */
#include "EcuM.h"
#include "Det.h"
#include "Dio.h"
#include "Gpt.h"
#include "../mcal/Mcal_Dio.h"
#include "../mcal/Mcal_Gpt.h"
#include <stdio.h>

static EcuM_StateType s_state = ECUM_STATE_OFF;

void EcuM_Init(void)
{
    printf("\n[BSW ][EcuM] ======= STARTUP PHASE ONE =======\n");
    printf("[BSW ][EcuM] Initializing MCAL layer...\n");

    /* MCAL init */
    Mcal_Dio_Init();
    Mcal_Gpt_Init();

    s_state = ECUM_STATE_STARTUP;
    printf("[BSW ][EcuM] MCAL init complete.\n");
}

void EcuM_StartupTwo(void)
{
    printf("[BSW ][EcuM] ======= STARTUP PHASE TWO =======\n");
    printf("[BSW ][EcuM] Initializing BSW layer...\n");

    /* BSW init */
    Det_Init();
    Dio_Init();
    Gpt_Init();

    s_state = ECUM_STATE_RUN;
    printf("[BSW ][EcuM] BSW init complete. ECU is RUN state.\n\n");
}

EcuM_StateType EcuM_GetState(void)
{
    return s_state;
}

void EcuM_Shutdown(void)
{
    printf("\n[BSW ][EcuM] ======= SHUTDOWN =======\n");
    Det_PrintLog();
    s_state = ECUM_STATE_SHUTDOWN;
    printf("[BSW ][EcuM] Shutdown complete.\n");
}
