# Architecture – Mini-AUTOSAR FLA

## Layer responsibilities

### MCAL (Microcontroller Abstraction Layer)
- Closest to hardware (or mock hardware in this project).
- `Mcal_Dio`: an `uint8_t` array acts as the GPIO register bank.
- `Mcal_Gpt`: a `uint32_t` counter acts as the system timer.
- **No MCAL header is ever included above BSW level.**

### BSW (Basic Software)
- **Dio**: translates symbolic channel names (from `Dio_Cfg.h`) to MCAL calls. Enforces direction. Reports `DET_E_PARAM_INVALID` on bad channel.
- **Gpt**: thin wrapper over `Mcal_Gpt`, exposes `Gpt_Tick()` to scheduler.
- **Det**: records module/API/error-code tuples in a ring buffer; prints on shutdown.
- **EcuM**: `Init()` → MCAL init; `StartupTwo()` → BSW init. Sets state machine to RUN.

### RTE (Runtime Environment)
- Owns a `Rte_DataBuffer_t` struct. All inter-SWC data lives here.
- Exposes typed `Rte_Read_*/Rte_Write_*` functions — the only API SWCs may call.
- `Rte_Actuate()` flushes computed LED values to `Dio_WriteChannel`.
- All configuration (which RTE signal maps to which channel) lives in `Rte_Cfg.h`, which is **generated** from `ecu_config.yaml`.

### Application Layer (SWCs)
- Each SWC is a plain `.c/.h` pair with `_Init()` and `_Run()` functions.
- SWCs **never** include BSW or MCAL headers. They only include `Rte.h`.
- `ButtonReader`: debounces raw GPIO via a counter; writes stable state.
- `BlinkController`: truth-table decision + tick-based blink state machine.
- `Heartbeat`: simple toggle on each `_Run()` invocation.

## Startup sequence

```
main()
  └─ EcuM_Init()
       ├─ Mcal_Dio_Init()        ← clear GPIO bank
       └─ Mcal_Gpt_Init()        ← reset tick counter
  └─ EcuM_StartupTwo()
       ├─ Det_Init()
       ├─ Dio_Init()             ← apply pin directions from Dio_Cfg.h
       └─ Gpt_Init()
  └─ Rte_Init()                  ← clear signal buffer
  └─ SWC_Init() × 3
  └─ scheduler loop (tick-based)
       ├─ Gpt_Tick()
       ├─ SimulateInputs()       ← test helper
       ├─ ButtonReader_Run()     ← 10ms
       ├─ BlinkController_Run()  ← 10ms
       ├─ Heartbeat_Run()        ← 500ms
       └─ Rte_Actuate()          ← flush to GPIO
  └─ EcuM_Shutdown()
       └─ Det_PrintLog()
```

## Data flow for one scheduler cycle

```
[virtual GPIO bank]
       │  Mcal_Dio_ReadChannel()
       ▼
[BSW Dio]  Dio_ReadChannel()
       │
       ▼
[RTE]  Rte_Read_ButtonReader_Button0()
       │  returns ButtonState_t
       ▼
[SWC ButtonReader]  debounce logic
       │  Rte_Write_ButtonReader_Btn0State()
       ▼
[RTE buffer]  buf.btn0State updated
       │  Rte_Read_BlinkCtrl_Btn0State()
       ▼
[SWC BlinkController]  decision + blink SM
       │  Rte_Write_BlinkCtrl_LedBlink()
       ▼
[RTE buffer]  buf.ledBlink updated
       │  Rte_Actuate() → Dio_WriteChannel()
       ▼
[BSW Dio]  → Mcal_Dio_WriteChannel()
       ▼
[virtual GPIO bank]  pin_level[3] = 0 or 1
```
