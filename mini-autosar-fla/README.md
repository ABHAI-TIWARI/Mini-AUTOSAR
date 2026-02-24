# Mini-AUTOSAR FLA 🚗

> A platform-agnostic **Full Layered Architecture** demo in plain C — no DaVinci, no EB Tresos, no licence fees.

[![Build](https://img.shields.io/badge/build-CMake-blue)](#building)
[![C Standard](https://img.shields.io/badge/C-C11-green)](#)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey)](#)

---

## Why this project exists

Real AUTOSAR toolchains cost tens of thousands of euros and require vendor licences.  
This project shows that the **core concepts** — layered architecture, RTE signal passing, MCAL abstraction, EcuM startup, DET error tracing, and config-driven code generation — can be taught and demonstrated with nothing but **gcc, CMake, and a text editor**.

Clone it, run it, read the 600 lines of C, and you will understand AUTOSAR FLA better than most spec documents explain it.

---

## Architecture

```
┌─────────────────────────────────────────────────────┐
│                 APPLICATION LAYER                   │
│  ┌─────────────┐ ┌──────────────┐ ┌─────────────┐  │
│  │ ButtonReader│ │BlinkController│ │  Heartbeat  │  │
│  └──────┬──────┘ └──────┬───────┘ └──────┬──────┘  │
│         │  RTE API only │               │          │
├─────────▼───────────────▼───────────────▼──────────┤
│              RUNTIME ENVIRONMENT (RTE)              │
│  Signal buffers · Port mapping · Runnable scheduler │
├─────────────────────────────────────────────────────┤
│              BSW – BASIC SOFTWARE                   │
│   Dio  │  Gpt  │  Det (error tracer)  │  EcuM init  │
├─────────────────────────────────────────────────────┤
│        MCAL – Microcontroller Abstraction Layer     │
│    Mcal_Dio (virtual GPIO bank) · Mcal_Gpt (tick)   │
└─────────────────────────────────────────────────────┘
         ▲
         │  No real hardware. GPIO = in-memory struct.
```

---

## Demo scenario

The main executable simulates 7.5 seconds of ECU runtime:

| Tick (ms)   | BTN0 | BTN1 | LED_BLINK behaviour     |
|-------------|------|------|-------------------------|
| 0 – 999     | ○    | ○    | **OFF**                 |
| 1000 – 2999 | ●    | ○    | **Slow blink** (1 Hz)   |
| 3000 – 3999 | ●    | ●    | **Fast blink** (5 Hz)   |
| 4000 – 4999 | ○    | ●    | **Fast blink** (BTN1)   |
| 5000 – 5999 | ●    | ●    | **Solid ON**            |
| 6000+       | ○    | ○    | **OFF**                 |
| 7000        | —    | —    | DET stress test fires   |

Every 500 ms the Heartbeat SWC toggles independently, printing `♥ tick`.

---

## Repo structure

```
mini-autosar-fla/
├── config/
│   └── ecu_config.yaml       ← Single source of truth for pin assignments
├── docs/
│   ├── architecture.md       ← This overview
│   └── modules/              ← Per-module spec (requirements + APIs)
├── scripts/
│   └── gen_config.py         ← Generates Dio_Cfg.h + Rte_Cfg.h from YAML
├── src/
│   ├── mcal/                 ← Mock hardware abstraction
│   │   ├── Mcal_Dio.h/c      ← Virtual GPIO register bank
│   │   └── Mcal_Gpt.h/c      ← Software tick counter
│   ├── bsw/                  ← Basic software services
│   │   ├── Dio.h/c           ← Digital I/O service
│   │   ├── Gpt.h/c           ← Timer service
│   │   ├── Det.h/c           ← Development Error Tracer
│   │   ├── EcuM.h/c          ← ECU Manager (startup/shutdown)
│   │   ├── Dio_Cfg.h         ← ← GENERATED from ecu_config.yaml
│   ├── rte/
│   │   ├── Rte.h/c           ← RTE API + signal buffer
│   │   ├── Rte_Cfg.h         ← ← GENERATED from ecu_config.yaml
│   │   └── Rte_Types.h       ← Shared signal types
│   └── app/                  ← SWCs – only talk to RTE
│       ├── ButtonReader.h/c
│       ├── BlinkController.h/c
│       └── Heartbeat.h/c
├── tests/                    ← Unit tests (custom minitest framework)
│   ├── minitest.h            ← ~50-line test framework, zero dependencies
│   ├── test_Mcal_Dio.c
│   ├── test_ButtonReader.c
│   ├── test_BlinkController.c
│   ├── test_Heartbeat.c
│   └── test_Det.c
├── main.c                    ← Scheduler + demo scenario
└── CMakeLists.txt
```

---

## Building

### Prerequisites
- CMake ≥ 3.16
- gcc or clang (any C11 compiler)
- Python 3 + PyYAML (only for config regeneration)

### Build & run
```bash
git clone : 
cd mini-autosar-fla

cmake -B build
cmake --build build

./build/mini_autosar_fla
```

### Run unit tests
```bash
cd build && ctest -V
```

### Regenerate configuration headers from YAML
```bash
pip install pyyaml
python3 scripts/gen_config.py
cmake --build build   # rebuild after config change
```

---

## The "config vs code" demo

Open `config/ecu_config.yaml` and change the heartbeat LED from channel `4` to channel `5`:

```yaml
# Before
- name: LED_HEARTBEAT
  channel: 4

# After
- name: LED_HEARTBEAT
  channel: 5
```

Then run:
```bash
python3 scripts/gen_config.py && cmake --build build
```

The **Heartbeat SWC source code is untouched**.  
Only the generated `Rte_Cfg.h` and `Dio_Cfg.h` change.  
This is exactly how AUTOSAR configuration separation works.

---

## Key AUTOSAR concepts demonstrated

| Concept | Where in this project |
|---|---|
| Layered architecture (MCAL/BSW/RTE/App) | Entire `src/` structure + CMake libs |
| SWC isolation via RTE | App SWCs never `#include` BSW headers |
| EcuM startup sequence | `EcuM_Init()` → `EcuM_StartupTwo()` |
| DET development error tracing | `Det_ReportError()` on bad channel |
| Config-driven code generation | `gen_config.py` → `Dio_Cfg.h` / `Rte_Cfg.h` |
| Runnable scheduling | `main.c` period-based `if (tick % period == 0)` |
| Debounce in SWC | `ButtonReader.c` counter-based debounce |
| Blink state machine | `BlinkController.c` tick-based toggling |

---

## Extending this project

Possible next steps to make it even more impressive:

- [ ] Add `Com` (Communication) BSW module with a virtual CAN frame
- [ ] Add `NvM` (NV Memory) mock with file-backed storage
- [ ] Port MCAL to a real STM32 or ESP32 target (only MCAL changes)
- [ ] Add `Os` – a tiny OSEK-like task table replacing the manual `if` scheduler
- [ ] Integrate with **ceedling/Unity** or **GoogleTest** for richer test output
- [ ] Generate a sequence diagram from log output with a Python post-processor

---

## References

- [AUTOSAR Classic Platform Specification](https://www.autosar.org/standards/classic-platform)
- [AUTOSAR SWS DIO Driver](https://www.autosar.org/fileadmin/standards/R22-11/CP/AUTOSAR_SWS_DIODriver.pdf)
- [AUTOSAR Layered Architecture](https://www.autosar.org/fileadmin/standards/R22-11/CP/AUTOSAR_EXP_LayeredSoftwareArchitecture.pdf)

---

*Built to showcase AUTOSAR knowledge without commercial tooling. — ABHAI*
