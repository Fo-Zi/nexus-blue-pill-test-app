cmake_minimum_required(VERSION 3.16)

# Enable ASM for startup file
project(blue_pill_test_app LANGUAGES C ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
set(INCLUDE_DIRS
    include
    platform
    ../hal-interface/include
    ../nexus-llhal-stm32f103/include
    ../nexus-llhal-stm32f103/src/internal
)

# Source files
set(SOURCES
    src/main.c
    src/startup_stm32f103c8tx.s
    platform/platform_config.c
)

# Create executable
add_executable(blue_pill_test_app ${SOURCES})

# Include directories
target_include_directories(blue_pill_test_app PRIVATE ${INCLUDE_DIRS})

# Link with STM32F103 HAL library
target_link_libraries(blue_pill_test_app nexus-llhal-stm32f103)

# Add nexus-llhal-stm32f103 as a dependency
add_subdirectory(../nexus-llhal-stm32f103 nexus-llhal-stm32f103)

# Compiler flags for STM32F103
target_compile_definitions(blue_pill_test_app PRIVATE
    STM32F103xB
    STM32F103
)

target_compile_options(blue_pill_test_app PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -fdata-sections
    -ffunction-sections
)

# ARM-specific flags for embedded target
target_compile_options(blue_pill_test_app PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
)

target_link_options(blue_pill_test_app PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
    -Wl,--gc-sections
    -specs=nosys.specs
    -specs=nano.specs
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/blue_pill_test_app.map,--cref
)

# Generate binary file for flashing
add_custom_command(TARGET blue_pill_test_app POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:blue_pill_test_app> ${CMAKE_CURRENT_BINARY_DIR}/blue_pill_test_app.bin
    COMMENT "Generating binary file"
)

# Flash target using OpenOCD
add_custom_target(flash
    COMMAND openocd -f ${CMAKE_CURRENT_SOURCE_DIR}/openocd.cfg -c "program ${CMAKE_CURRENT_BINARY_DIR}/blue_pill_test_app.bin 0x08000000 verify reset exit"
    DEPENDS blue_pill_test_app
    COMMENT "Flashing blue_pill_test_app.bin to Blue Pill via OpenOCD"
)