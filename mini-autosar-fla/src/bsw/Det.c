/**
 * @file    Det.c
 * @brief   Mini-AUTOSAR BSW – Development Error Tracer implementation
 */
#include "Det.h"
#include <stdio.h>
#include <string.h>

static Det_ErrorRecord_t s_errors[DET_MAX_ERRORS];
static uint8_t           s_count = 0u;
static uint8_t           s_initialized = 0u;

void Det_Init(void)
{
    memset(s_errors, 0, sizeof(s_errors));
    s_count = 0u;
    s_initialized = 1u;
    printf("[BSW ][Det]  Init – DET active, max %u records.\n", DET_MAX_ERRORS);
}

void Det_ReportError(uint8_t moduleId, uint8_t instanceId,
                     uint8_t apiId,    uint8_t errorId)
{
    if (!s_initialized) {
        printf("[BSW ][Det]  ERROR reported before Det_Init()!\n");
        return;
    }

    uint8_t idx = s_count % DET_MAX_ERRORS;
    s_errors[idx].moduleId   = moduleId;
    s_errors[idx].instanceId = instanceId;
    s_errors[idx].apiId      = apiId;
    s_errors[idx].errorId    = errorId;
    s_count++;

    printf("[BSW ][DET] *** Error: Module=0x%02X Inst=0x%02X API=0x%02X Err=0x%02X ***\n",
           moduleId, instanceId, apiId, errorId);
}

void Det_PrintLog(void)
{
    printf("[BSW ][Det]  --- Error Log (%u total) ---\n", s_count);
    uint8_t n = (s_count < DET_MAX_ERRORS) ? s_count : DET_MAX_ERRORS;
    for (uint8_t i = 0u; i < n; i++) {
        printf("  [%02u] Mod=0x%02X Inst=0x%02X API=0x%02X Err=0x%02X\n",
               i,
               s_errors[i].moduleId,
               s_errors[i].instanceId,
               s_errors[i].apiId,
               s_errors[i].errorId);
    }
}

uint8_t Det_GetErrorCount(void)
{
    return s_count;
}
