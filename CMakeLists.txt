cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(NAME badger2040-dvdbounce) # <-- Name your project/executable here!

set(PICO_SDK_PATH "../../pico-sdk")
set(PIMORONI_PICO_PATH "../../pimoroni-pico")
set(PICO_BOARD "pimoroni_badger2040")

include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add your source files
add_executable(${NAME}
    main.cpp
)

include(libraries/badger2040/badger2040)

target_link_libraries(${NAME}
    badger2040
    hardware_spi
    pico_stdlib
)

# enable usb output
pico_enable_stdio_usb(${NAME} 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    DESTINATION .
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
