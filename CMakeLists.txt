# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(erm19264 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
  #examples/ERM19264_UC1609_HELLO/main.cpp
  examples/ERM19264_UC1609_MISC/main.cpp
  #examples/ERM19264_UC1609_BITMAP/main.cpp
 
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

add_library(pico_erm19264 INTERFACE)

target_sources(pico_erm19264 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/erm19264/ERM19264_UC1609.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/erm19264/ERM19264_UC1609_graphics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/erm19264/ERM19264_UC1609_Print.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/erm19264/ERM19264_UC1609_font.cpp
)

target_include_directories(pico_erm19264 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi pico_erm19264 )


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

