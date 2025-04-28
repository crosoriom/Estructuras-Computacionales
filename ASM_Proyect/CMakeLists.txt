cmake_minimum_required(VERSION 3.22)

project(ASM_Proyect LANGUAGES C ASM) # Enable languages directly here

# --- Toolchain Configuration ---
set(CMAKE_SYSTEM_NAME            Generic)
set(CMAKE_SYSTEM_PROCESSOR       ARM)
set(TOOLCHAIN_PREFIX           arm-none-eabi-)

# Explicitly set compilers - Ensure these are found in your PATH or provide full paths
set(CMAKE_C_COMPILER           ${TOOLCHAIN_PREFIX}gcc CACHE FILEPATH "C compiler")
set(CMAKE_OBJCOPY              ${TOOLCHAIN_PREFIX}objcopy CACHE FILEPATH "obj utility")
set(CMAKE_SIZE                 ${TOOLCHAIN_PREFIX}size CACHE FILEPATH "size utility")

# --- Target Configuration ---
# Use modern -march/-mtune instead of deprecated -mcpu
# Cortex-M4 is ARMv7E-M architecture
set(TARGET_ARCH                armv7e-m CACHE STRING "Target ARM Architecture (e.g., armv7e-m)")
set(TARGET_TUNE                cortex-m4 CACHE STRING "Target CPU Tuning (e.g., cortex-m4)")
set(TARGET_FPU                 fpv4-sp-d16 CACHE STRING "Target FPU (e.g., soft, fpv4-sp-d16, fpv5-sp-d16)") # Common for L476 if FPU is used
set(TARGET_FLOAT_ABI           hard CACHE STRING "Target Float ABI (soft or hard)") # Use 'hard' if using FPU, 'soft' otherwise

set(OPTIMIZATION_LEVEL         O2 CACHE STRING "Compiler Optimization (e.g., O0, O1, O2, O3, Os, Og)")

# --- File Paths ---
set(LINKER_SCRIPT_PATH         ${CMAKE_SOURCE_DIR}/STM32L476RGTX_FLASH.ld)
set(STARTUP_FILE               ${CMAKE_SOURCE_DIR}/startup_stm32l476xx.s)

if(NOT EXISTS ${LINKER_SCRIPT_PATH})
    message(FATAL_ERROR "Linker file not found: ${LINKER_SCRIPT_PATH}")
endif()
if(NOT EXISTS ${STARTUP_FILE})
    message(FATAL_ERROR "Startup file not found: ${STARTUP_FILE}")
endif()

# --- Compiler Flags ---
# Common flags for both C and ASM
add_compile_options(
    -march=${TARGET_ARCH}      # Use -march
    -mtune=${TARGET_TUNE}      # Use -mtune
    -mthumb                    # Use Thumb instruction set
    -mfpu=${TARGET_FPU}        # Specify FPU hardware
    -mfloat-abi=${TARGET_FLOAT_ABI} # Specify float ABI (match FPU setting)
    -fdata-sections            # Place data sections in separate sections
    -ffunction-sections        # Place function sections in separate sections
    -Wall                      # Enable common warnings
    -Wextra                    # Enable extra warnings
    #-g                         # Uncomment for debug information
)

# C specific flags
add_compile_options($<$<COMPILE_LANGUAGE:C>:-${OPTIMIZATION_LEVEL}>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-std=c11>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-Wno-unused-parameter>) # Keep if needed

# ASM specific flags (tell gcc to treat .s as assembly needing potential preprocessing)
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x> $<$<COMPILE_LANGUAGE:ASM>:assembler-with-cpp>)

# --- Linker Flags ---
# Many flags are passed implicitly by the compiler driver (gcc) based on compile options
# Only add necessary linker-specific flags here
set(LINKER_FLAGS
    -march=${TARGET_ARCH}      # Pass arch to linker
    -mtune=${TARGET_TUNE}      # Pass tune to linker
    -mthumb
    -mfpu=${TARGET_FPU}
    -mfloat-abi=${TARGET_FLOAT_ABI}
    -T${LINKER_SCRIPT_PATH}    # *** Corrected variable name ***
    -Wl,-Map=${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map,--cref # Output map file to build dir
    -Wl,--gc-sections          # Garbage collect unused sections
    --specs=nosys.specs        # Link with minimal syscall stubs
    -Wl,--start-group          # Linker group for libraries
    -lc                        # Link C library
    -lm                        # Link Math library
    -Wl,--end-group
    -Wl,--print-memory-usage
)

# --- Project Sources ---
set(SOURCES
    ${STARTUP_FILE}            # Startup file first
    ${CMAKE_SOURCE_DIR}/src/main.s
    ${CMAKE_SOURCE_DIR}/User/syscalls.c
    ${CMAKE_SOURCE_DIR}/User/sysmem.c
)

# --- Executable Target ---
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# --- Include Directories ---
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/User
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/src
)

# --- Preprocessor Definitions ---
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add definitions like USE_HAL_DRIVER etc. if needed
    # DEBUG # Example definition
)

# --- Link Options ---
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE ${LINKER_FLAGS})

# --- Post-Build Steps ---
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex # Output hex to build dir
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin # Output bin to build dir
    COMMENT "Generating .hex, .bin files and printing size"
)

# --- Optional: Export compile commands for tooling ---
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


