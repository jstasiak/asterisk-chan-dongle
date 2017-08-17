cmake_minimum_required(VERSION 3.8)

project (asterisk-chan-dongle)

set(Asterisk_INCLUDE_DIR /home/oleg/workspace_gcc/Include/Asterisk)

# set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include_directories(${Asterisk_INCLUDE_DIR})

file(GLOB CF
        app.c at_command.c at_parse.c at_queue.c at_read.c at_response.c
        chan_dongle.c channel.c char_conv.c cli.c cpvt.c dc_config.c helpers.c
        manager.c memmem.c ringbuffer.c pdu.c mixbuffer.c pdiscovery.c
        )
file(GLOB HF
        app.h at_command.h at_parse.h at_queue.h at_read.h at_response.h
        chan_dongle.h channel.h char_conv.h cli.h cpvt.h dc_config.h export.h
        helpers.h manager.h memmem.h ringbuffer.h pdu.h mixbuffer.h pdiscovery.h
        mutils.h
        )

add_definitions(-DASTERISK_VERSION_NUM=130000)
add_definitions(-DBUILD_APPLICATIONS)
#add_definitions(-DAST_MODULE="ast")
#add_definitions(-DPACKAGE_REVISION="3")
#add_definitions(-DMODULE_VERSION="1")
#add_definitions(-DMODULE_BUGREPORT="")
#add_definitions(-DMODULE_URL="")

# name of asterisk module 
add_definitions(-DAST_MODULE="chan_dongle")

# Define to 1 if you have HAVE_AST_CONTROL_SRCCHANGE in asterisk/frame.h
add_definitions(-DHAVE_AST_CONTROL_SRCCHANGE)

# Define to the address where bug reports for this package should be sent. 
add_definitions(-DMODULE_BUGREPORT="http://code.google.com/p/asterisk-chan-dongle/issues/list")

# Define to the full name of this package.
# undef PACKAGE_NAME

# Revision of package 
add_definitions(-DPACKAGE_REVISION="34")

# Define to the full name and version of this package. 
#undef PACKAGE_STRING

# Define to the one symbol short name of this package. 
#undef PACKAGE_TARNAME

# Define to the home page for this package. 
add_definitions(-DMODULE_URL="http://code.google.com/p/asterisk-chan-dongle")

# Define to the version of this package. 
add_definitions(-DMODULE_VERSION="1.1")


FIND_PACKAGE(Iconv REQUIRED)
MESSAGE("ICONV_INCLUDE_DIR: " ${ICONV_INCLUDE_DIR})
MESSAGE("ICONV_LIBRARIES: " ${ICONV_LIBRARIES})
include_directories(${ICONV_INCLUDE_DIR})
add_definitions(-DICONV_CONST=const)
add_definitions(-DICONV_T=iconv_t)

add_library(${PROJECT_NAME} SHARED ${CF} ${HF})

target_link_libraries(${PROJECT_NAME} ${ICONV_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES
        CLEAN_DIRECT_OUTPUT 1
        OUTPUT_NAME "chan_dongle"
        PREFIX ""
        )