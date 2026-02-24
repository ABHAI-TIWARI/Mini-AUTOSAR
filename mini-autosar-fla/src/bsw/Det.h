/**
 * @file    Det.h
 * @brief   Mini-AUTOSAR BSW – Development Error Tracer
 * @version 1.0.0
 * @ref     AUTOSAR_SWS_DefaultErrorTracer
 *
 * DET records module errors during development builds.
 * In production, these calls can compile away.
 */
#ifndef DET_H
#define DET_H

#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Error codes (example subset)                                       */
/* ------------------------------------------------------------------ */
#define DET_E_PARAM_INVALID   0x01u
#define DET_E_UNINIT          0x02u
#define DET_E_BUFFER_OVERFLOW 0x03u

/* Maximum stored errors before overwrite */
#define DET_MAX_ERRORS  16u

typedef struct {
    uint8_t  moduleId;
    uint8_t  instanceId;
    uint8_t  apiId;
    uint8_t  errorId;
} Det_ErrorRecord_t;

/* ------------------------------------------------------------------ */
/*  API                                                                 */
/* ------------------------------------------------------------------ */
void Det_Init(void);
void Det_ReportError(uint8_t moduleId, uint8_t instanceId,
                     uint8_t apiId,    uint8_t errorId);
void Det_PrintLog(void);
uint8_t Det_GetErrorCount(void);

#endif /* DET_H */
