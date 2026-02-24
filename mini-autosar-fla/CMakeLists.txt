cmake_minimum_required(VERSION 3.16)
project(MiniAutosarFLA C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ─── Compiler warnings ──────────────────────────────────────────────
add_compile_options(
    -Wall -Wextra -Wpedantic
    -Wno-unused-parameter
)

# ─── Source file lists ──────────────────────────────────────────────
set(MCAL_SOURCES
    src/mcal/Mcal_Dio.c
    src/mcal/Mcal_Gpt.c
)

set(BSW_SOURCES
    src/bsw/Det.c
    src/bsw/Dio.c
    src/bsw/Gpt.c
    src/bsw/EcuM.c
)

set(RTE_SOURCES
    src/rte/Rte.c
)

set(APP_SOURCES
    src/app/ButtonReader.c
    src/app/BlinkController.c
    src/app/Heartbeat.c
)

# ─── Static libraries per layer (mirrors AUTOSAR layered arch) ──────
add_library(mcal STATIC ${MCAL_SOURCES})
target_include_directories(mcal PUBLIC src/mcal)

add_library(bsw STATIC ${BSW_SOURCES})
target_include_directories(bsw PUBLIC src/bsw src/mcal)
target_link_libraries(bsw mcal)

add_library(rte STATIC ${RTE_SOURCES})
target_include_directories(rte PUBLIC src/rte src/bsw src/mcal)
target_link_libraries(rte bsw)

add_library(app STATIC ${APP_SOURCES})
target_include_directories(app PUBLIC src/app src/rte src/bsw src/mcal)
target_link_libraries(app rte)

# ─── Main executable ────────────────────────────────────────────────
add_executable(mini_autosar_fla main.c)
target_include_directories(mini_autosar_fla PRIVATE .)
target_link_libraries(mini_autosar_fla app rte bsw mcal)

# ─── Unit tests ─────────────────────────────────────────────────────
enable_testing()
add_subdirectory(tests)

# ─── Convenience target: build + run demo ───────────────────────────
add_custom_target(demo
    COMMAND mini_autosar_fla
    DEPENDS mini_autosar_fla
    COMMENT "Running Mini-AUTOSAR FLA demo..."
)

message(STATUS "")
message(STATUS "  Mini-AUTOSAR FLA build configured")
message(STATUS "  Build:  cmake -B build && cmake --build build")
message(STATUS "  Run:    ./build/mini_autosar_fla")
message(STATUS "  Test:   cd build && ctest -V")
message(STATUS "")
