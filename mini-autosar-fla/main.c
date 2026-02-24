/**
 * @file    main.c
 * @brief   Mini-AUTOSAR FLA – Demo entry point
 *
 * Demonstrates a simulated scenario:
 *   Tick   0- 999 ms : No buttons pressed    → LED off
 *   Tick 1000-2999 ms : BTN0 pressed          → Slow blink
 *   Tick 3000-3999 ms : BTN1 also pressed     → Fast blink
 *   Tick 4000-4999 ms : BTN0 released         → Fast blink (BTN1 only)
 *   Tick 5000-5999 ms : Both pressed          → Solid on
 *   Tick 6000+       : Both released           → Off
 *   At tick 7000      : DET stress test (bad channel)
 *
 * Layered call flow:
 *   main → Scheduler → RTE → BSW/Dio → MCAL
 *                    ↗
 *   App SWCs ───────
 */
#include <stdio.h>
#include <stdint.h>

#include "src/bsw/EcuM.h"
#include "src/bsw/Gpt.h"
#include "src/bsw/Det.h"
#include "src/rte/Rte.h"
#include "src/app/ButtonReader.h"
#include "src/app/BlinkController.h"
#include "src/app/Heartbeat.h"
#include "src/mcal/Mcal_Dio.h"

/* ------------------------------------------------------------------ */
/*  Scenario: inject button states at specific ticks                  */
/* ------------------------------------------------------------------ */
static void SimulateInputs(uint32_t tick)
{
    uint8_t btn0 = 0u;
    uint8_t btn1 = 0u;

    if (tick >= 1000u && tick < 5000u) btn0 = 1u;
    if (tick >= 3000u && tick < 6000u) btn1 = 1u;

    /* Inject directly into virtual GPIO bank (bypasses direction check) */
    Mcal_Dio_InjectInput(0u, btn0 ? MCAL_DIO_HIGH : MCAL_DIO_LOW);
    Mcal_Dio_InjectInput(1u, btn1 ? MCAL_DIO_HIGH : MCAL_DIO_LOW);
}

/* ------------------------------------------------------------------ */
/*  Scheduler: runnable periods (ms)                                  */
/* ------------------------------------------------------------------ */
#define SCHED_PERIOD_BTN_MS  10u
#define SCHED_PERIOD_HB_MS   500u

/* ------------------------------------------------------------------ */
/*  Main                                                              */
/* ------------------------------------------------------------------ */
int main(void)
{
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║      Mini-AUTOSAR FLA – Demo Startup             ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    /* ----- EcuM startup sequence ----------------------------------- */
    EcuM_Init();          /* MCAL layer init            */
    EcuM_StartupTwo();    /* BSW layer init + Det        */

    /* ----- RTE init ------------------------------------------------- */
    Rte_Init();
    printf("[RTE ][Sched] Runnable periods: BTN/Blink=%ums  HB=%ums\n\n",
           SCHED_PERIOD_BTN_MS, SCHED_PERIOD_HB_MS);

    /* ----- SWC init ------------------------------------------------- */
    ButtonReader_Init();
    BlinkController_Init();
    Heartbeat_Init();

    printf("\n[SCHED] ======= RUN PHASE START =======\n\n");

    /* ----- Main scheduler loop (simulate 7500 ticks = 7.5 seconds) -- */
    const uint32_t SIM_TICKS = 7500u;

    for (uint32_t tick = 0u; tick < SIM_TICKS; tick++)
    {
        /* Advance system tick */
        Gpt_Tick();

        /* Inject simulated button presses */
        SimulateInputs(tick);

        /* --- 10 ms tasks ------------------------------------------- */
        if (tick % SCHED_PERIOD_BTN_MS == 0u) {
            ButtonReader_Run();
            BlinkController_Run();
        }

        /* --- 500 ms tasks ------------------------------------------ */
        if (tick % SCHED_PERIOD_HB_MS == 0u && tick > 0u) {
            Heartbeat_Run();

            /* Print LED state summary every heartbeat tick */
            const Rte_DataBuffer_t *buf = Rte_GetBuffer();
            printf("[SCHED] Tick=%-5u  LED_BLINK=%u  LED_STATUS=%u  "
                   "LED_HB=%u  Pattern=%u\n",
                   tick,
                   buf->ledBlink,
                   buf->ledStatus,
                   buf->ledHeartbeat,
                   (uint8_t)buf->blinkPattern);
        }

        /* Flush computed LED values to virtual GPIO (BSW/Dio) */
        Rte_Actuate();

        /* DET stress test at tick 7000: write to invalid channel */
        if (tick == 7000u) {
            printf("\n[SCHED] *** Injecting DET test: invalid channel write ***\n");
            /* DIO_MODULE_ID will report DET_E_PARAM_INVALID */
            extern void Dio_WriteChannel(uint8_t ch, uint8_t lvl);
            Dio_WriteChannel(0xFFu, 1u);   /* channel out of range */
        }
    }

    /* ----- EcuM shutdown ------------------------------------------- */
    EcuM_Shutdown();

    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║  Simulation complete. Clone and run it yourself! ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    return 0;
}
