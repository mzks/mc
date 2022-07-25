cmake_minimum_required(VERSION 3.13)
enable_testing()

# Disable build at top-level of sources
get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)
if("${srcdir}" STREQUAL "${bindir}")
    message("Do not build on the top level source.")
    message(FATAL_ERROR "Hint: git clean -d -f -x -n")
endif()

project(MC)

find_package(Git)
if(NOT Git_FOUND)
  message("Git not found")
endif()

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

add_subdirectory(mc)
add_subdirectory(analysis)

add_custom_target(alltar DEPENDS common mc analysis)


## Tests
add_test(NAME small_mc COMMAND ../bin/mc -p ../bench -m run.mac -o mc_small.root)
add_test(NAME small_mc2 COMMAND ../bin/mc -p ../bench -m run.mac -o mc_small2.root)
add_test(NAME reproducibility COMMAND ../bin/reproducibility -i mc_small.root -j mc_small2.root -t tree)
add_test(NAME batch_mc COMMAND ../bin/mc -p ../bench -m bat.mac -o mc.root)
add_test(NAME mcProcessing COMMAND ../bin/mcProcessing -i mc.root -o processed.root --clone)
add_test(NAME basic_check COMMAND ../bin/basic_check -i processed.root)
