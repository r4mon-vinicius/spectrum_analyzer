# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(spectrum_analyzer C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Adiciona a biblioteca do microfone
add_subdirectory(libs/mic)

# Adiciona a biblioteca KISS FFT
add_subdirectory(libs/fft)

# Adiciona a biblioteca do display oled
add_subdirectory(libs/oled)

# Add executable. Default name is the project name, version 0.1
add_executable(spectrum_analyzer spectrum_analyzer.c)

pico_set_program_name(spectrum_analyzer "spectrum_analyzer")
pico_set_program_version(spectrum_analyzer "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(spectrum_analyzer 0)
pico_enable_stdio_usb(spectrum_analyzer 1)

# Add the standard library to the build
target_link_libraries(spectrum_analyzer
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(spectrum_analyzer PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(spectrum_analyzer 
        hardware_dma
        hardware_pio
        hardware_timer
        hardware_clocks
        hardware_adc
        mic
        oled
        kiss_fft

        )

pico_add_extra_outputs(spectrum_analyzer)

