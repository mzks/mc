cmake_minimum_required(VERSION 3.13)

IF (APPLE)
   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
ENDIF()

project(analysis)
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)
include_directories(${PROJECT_SOURCE_DIR}/include)
set(CMAKE_INSTALL_PREFIX .)

# Setting ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT CONFIG REQUIRED COMPONENTS)
include(${ROOT_USE_FILE})

# Add the executable, and link it to the Geant4 libraries
add_definitions(${ROOT_CXX_FLAGS})

add_executable(mcProcessing mcProcessing.cc)
add_executable(basic_check basic_check.cc)
add_executable(reproducibility reproducibility.cc)

target_link_libraries(mcProcessing ${ROOT_LIBRARIES})
target_link_libraries(basic_check ${ROOT_LIBRARIES})
target_link_libraries(reproducibility ${ROOT_LIBRARIES})

add_subdirectory(../common common)
include_directories(../common)
target_include_directories(mcProcessing PUBLIC "${CMAKE_BINARY_DIR}") 
target_include_directories(basic_check PUBLIC "${CMAKE_BINARY_DIR}") 
target_include_directories(reproducibility PUBLIC "${CMAKE_BINARY_DIR}") 

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


install(TARGETS mcProcessing DESTINATION bin)
install(TARGETS basic_check DESTINATION bin)
install(TARGETS reproducibility DESTINATION bin)

