# Module Spec – BlinkController SWC

**Version:** 1.0.0  
**Layer:** Application  
**AUTOSAR reference:** SWS_RTE (runnable concept)

---

## Requirements

| ID | Requirement |
|----|-------------|
| BC-001 | The BlinkController shall read BTN0 and BTN1 states from the RTE each cycle. |
| BC-002 | When both buttons are released, the blink LED shall be commanded OFF. |
| BC-003 | When BTN0 only is pressed, the blink LED shall toggle at 1 Hz (slow). |
| BC-004 | When BTN1 only is pressed, the blink LED shall toggle at 5 Hz (fast). |
| BC-005 | When both buttons are pressed simultaneously, the blink LED shall be SOLID ON. |
| BC-006 | The status LED shall be ON if any button is active, OFF otherwise. |
| BC-007 | The blink pattern shall be written to the RTE buffer every cycle. |
| BC-008 | The blink state machine shall use the BSW Gpt tick, not a wall clock. |

---

## API

```c
void BlinkController_Init(void);  // Reset internal state
void BlinkController_Run(void);   // Called by scheduler every 10 ms
```

---

## Truth table

| BTN0 | BTN1 | Pattern     | LED_BLINK          |
|------|------|-------------|---------------------|
| 0    | 0    | BLINK_OFF   | Always 0            |
| 1    | 0    | BLINK_SLOW  | Toggle every 500 ms |
| 0    | 1    | BLINK_FAST  | Toggle every 100 ms |
| 1    | 1    | BLINK_SOLID | Always 1            |

---

## Timing budget

- Runnable period: **10 ms**
- Worst-case execution: O(1) – no loops, no memory allocation
- Stack depth: 2 frames (Run → Rte_Read)
