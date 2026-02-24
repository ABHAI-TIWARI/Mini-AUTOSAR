# Software Components Interaction Diagram

Rendered image:

![Mini-AUTOSAR software components interaction](https://www.plantuml.com/plantuml/svg/dLLHRzem47xFhpZnKdXWevhsieUfG5bR2TIAiF7GLSXrNB1mxCYvjAkc_VUT7H9Y24ZRIvox-yxVtPUpSvKZirYaAaA9Ii1CQjaVVb_SnyCvV9iEeG-nUS4tPWMCJPePBJJcSATHMCPH6Xr5-LRgZ5cM0gyGCRxJPbPmP7gjHAJ42m8Qi7AzGKYaY_ZSZF4jMmle3BDCISwS5-BbEE_0xmZs6q9dL20QFHSi4RO3B8Ucum2gfDwEZKPhbAhGeIzCk1KamhDWwA6r5Vs9eWOgvXkH59JyKtTJ9lK7tlFusjy4pnSJWirHL2XoDDeTnSkZ3YUyc7ceUmb3rzBu2ArXu2OhQwRr84EK0LfT6pNnR3oS7Z7FE5EhYiCPHpmUKP4vmn66yloGD4XCmSt3tJsCcDxk2fF66L5q0R6VjGnaOtfgyQ3V_-exXoz-MTrfYTrUv5qkjgkBmbMvhH0gh8BGQmKvmxEhQl5ccbkHRWIWvn59MKCRVKbzcfL6WE8q4tNy0kQ5HfcACvhiTrr8lgKivpZPBT-13ZPf7XPAQ9ehAg7RWyVB0QJvKnGWmwgTSnMabiRWuoKnEboeZmEDDqnheIZk_2U-OmYi06NpvVWGsXA8PCeA12LUXShFbRgqafvXhUWAfQ-DeaMVy7fm4eMUGvKEy6r2UGXQTS1yFdLNU9qx5ObBBFB_oVJ6FoS-CAHPrCSYrHSZ9TRlbJ2V1wKoUr0RopwDohjzFdT6Xfok4wSOF0h6Dy3VkH9FBMFcTwx71tvzEZTg8VZKh1LPGjGmdLo3AP16BdTdTMAjiO1qJALUGyPmKpFjhukvo8n53-tsw1SWzIjJCW5USbB3Lq8dxe_x5m00)

PlantUML source (`software_components_interaction.puml`):

```plantuml
@startuml
title Mini-AUTOSAR FLA - Software Components Interaction

skinparam componentStyle rectangle
left to right direction

package "Application SWCs" {
  component "ButtonReader" as ButtonReader
  component "BlinkController" as BlinkController
  component "Heartbeat" as Heartbeat
}

component "Scheduler\n(main.c)" as Scheduler
component "RTE\n(Rte.c)" as RTE

package "BSW" {
  component "EcuM" as EcuM
  component "Dio" as Dio
  component "Gpt" as Gpt
  component "Det" as Det
}

package "MCAL" {
  component "Mcal_Dio" as McalDio
  component "Mcal_Gpt" as McalGpt
}

component "Virtual GPIO Bank" as GpioBank

' Startup interaction
Scheduler --> EcuM : EcuM_Init()
EcuM --> McalDio : Mcal_Dio_Init()
EcuM --> McalGpt : Mcal_Gpt_Init()

Scheduler --> EcuM : EcuM_StartupTwo()
EcuM --> Det : Det_Init()
EcuM --> Dio : Dio_Init()
EcuM --> Gpt : Gpt_Init()

Scheduler --> RTE : Rte_Init()

' Runtime interaction
Scheduler --> Gpt : Gpt_Tick()
Gpt --> McalGpt : Mcal_Gpt_IncTick()

Scheduler --> ButtonReader : ButtonReader_Run() [10 ms]
ButtonReader --> RTE : Rte_Read_ButtonReader_Button0/1()
RTE --> Dio : Dio_ReadChannel()
Dio --> McalDio : Mcal_Dio_ReadChannel()
McalDio --> GpioBank : read input levels
ButtonReader --> RTE : Rte_Write_ButtonReader_Btn0/1State()

Scheduler --> BlinkController : BlinkController_Run() [10 ms]
BlinkController --> RTE : Rte_Read_BlinkCtrl_Btn0/1State()
BlinkController --> RTE : Rte_Write_BlinkCtrl_LedStatus()
BlinkController --> RTE : Rte_Write_BlinkCtrl_LedBlink()
BlinkController --> RTE : Rte_Write_BlinkCtrl_Pattern()

Scheduler --> Heartbeat : Heartbeat_Run() [500 ms]
Heartbeat --> RTE : Rte_Write_Heartbeat_LedHb()

Scheduler --> RTE : Rte_Actuate() [each cycle]
RTE --> Dio : Dio_WriteChannel() x3
Dio --> McalDio : Mcal_Dio_WriteChannel()
McalDio --> GpioBank : update LED outputs

' Error tracing path
Dio --> Det : Det_ReportError()\n(invalid channel)

@enduml
```
