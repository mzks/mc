cmake_minimum_required(VERSION 3.13)
enable_testing()

project(MC)

IF (APPLE)
   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
ENDIF()

### --  Load header-only libraries ------------------------------- #
include(FetchContent)

# spdlog -- simple logger
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.x # This tag is like main
)
FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif ()
include_directories(${spdlog_SOURCE_DIR}/include)

# argparse -- simple argument parser
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
        GIT_TAG        master
)
FetchContent_GetProperties(argparse)
if (NOT argparse_POPULATED)
    FetchContent_Populate(argparse)
    add_subdirectory(${argparse_SOURCE_DIR} ${argparse_BINARY_DIR})
endif ()
include_directories(${argparse_SOURCE_DIR}/include)

### -------------------------------------------------------------- #

add_subdirectory(common)
include_directories(common)

add_custom_command( OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/common/common.hh
                    WORKING_DIRECTORY ${WORKING_DIRECTORY} ERROR_QUIET)
                #add_custom_target(alltar
                #    COMMAND ${CMAKE_COMMAND}
                #    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/common/common.hh
                #    -Dinput_file=${CMAKE_CURRENT_SOURCE_DIR}/common/common.hh.in
                #    -Doutput_file=${CMAKE_CURRENT_BINARY_DIR}/common/common.hh
                #    -P ${CMAKE_CURRENT_SOURCE_DIR}/common/CMakeLists.txt)

add_subdirectory(mc)
add_subdirectory(analysis)

add_custom_target(alltar DEPENDS source)


## Tests
add_test(NAME small_mc COMMAND ../bin/mc -p ../bench -m run.mac -o mc_small.root)
add_test(NAME batch_mc COMMAND ../bin/mc -p ../bench -m bat.mac -o mc.root)
add_test(NAME mcProcessing COMMAND ../bin/mcProcessing -i mc.root -o processed.root)
