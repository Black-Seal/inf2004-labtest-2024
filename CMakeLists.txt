cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH "/Users/adrian/Desktop/GitHub/pico-sdk")
set(PICO_TOOLCHAIN_PATH "/usr/local/arm-gnu-toolchain-12.3.rel1-darwin-x86_64-arm-none-eabi/bin")

include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(labtest C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Add linker flags to handle `nosys.specs` issue
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=nano.specs -lc -lnosys")

# Initialize the Pico SDK
pico_sdk_init()

# Add executable and source files
add_executable(labtest
    main.c
    functions.c
)

# Link with the Pico standard library and hardware PWM
target_link_libraries(labtest 
    pico_stdlib
    pico_time
    hardware_pwm
)

# Enable USB serial output
pico_enable_stdio_usb(labtest 1)

# Enable UART for serial monitor output (optional)
pico_enable_stdio_uart(labtest 1)

# Create the output files (e.g., .uf2 file for flashing)
pico_add_extra_outputs(labtest)
