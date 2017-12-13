INCLUDE(CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(toolchain arm-linux-gnueabihf)

# # clang
# set(CMAKE_C_COMPILER clang)
# set(CMAKE_C_COMPILER_TARGET ${toolchain})

message(STATUS "Toolcahin : ${CMAKE_TOOLCHAIN_FILE}")

# gcc
set(CMAKE_C_COMPILER ${toolchain}-gcc)
set(CMAKE_CXX_COMPILER ${toolchain}-g++)

SET(COMMON_FLAGS "-mcpu=cortex-a7 -mtune=cortex-a7 -mfpu=vfpv4-d16 -funsafe-math-optimizations")

SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -g -std=gnu++11 -O1 -Wall")
SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -g -std=gnu99 -O1 -Wall")
SET(CMAKE_ASM_FLAGS "${COMMON_FLAGS}")

# Find the target environment prefix
# First see where gcc is keeping libc.a
execute_process(
        COMMAND ${CMAKE_C_COMPILER} -print-file-name=libc.a
        OUTPUT_VARIABLE CMAKE_INSTALL_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
# Strip the filename off
get_filename_component(CMAKE_INSTALL_PREFIX
        "${CMAKE_INSTALL_PREFIX}" PATH
        )
# Then find the canonical path to the directory one up from there
get_filename_component(CMAKE_INSTALL_PREFIX
        "${CMAKE_INSTALL_PREFIX}/.." REALPATH
        )
set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} CACHE FILEPATH
        "Install path prefix, prepended onto install directories.")

message(STATUS "Cross-compiling with the linux-arm toolchain")
message(STATUS "Toolchain prefix: ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_FIND_ROOT_PATH ${CMAKE_INSTALL_PREFIX})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
