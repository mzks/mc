cmake_minimum_required(VERSION 3.13)

project(MC)

IF (APPLE)
   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
ENDIF()

find_package(Git)
if(NOT Git_FOUND)
  message("Git not found")
endif()

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

# # argparse -- simple argument parser
# FetchContent_Declare(
#         argparse
#         GIT_REPOSITORY https://github.com/p-ranav/argparse.git
#         GIT_TAG        master
# )
# FetchContent_GetProperties(argparse)
# if (NOT argparse_POPULATED)
#     FetchContent_Populate(argparse)
#     add_subdirectory(${argparse_SOURCE_DIR} ${argparse_BINARY_DIR})
# endif ()
# include_directories(${argparse_SOURCE_DIR}/include)

### -------------------------------------------------------------- #

add_subdirectory(common)
include_directories(common)

add_subdirectory(sub1)

add_custom_target(alltar DEPENDS common sub1)

