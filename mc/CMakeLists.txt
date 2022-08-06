cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

IF (APPLE)
   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
ENDIF()

project(mc)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
include_directories(${PROJECT_SOURCE_DIR}/include)


# Setting ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT CONFIG REQUIRED)
include(${ROOT_USE_FILE})

# Setting Geant4
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})

# Add the executable, and link it to the Geant4 libraries
add_definitions(${ROOT_CXX_FLAGS})
add_executable(mc mc.cc ${sources} ${headers})
target_link_libraries(mc PRIVATE ROOT::MathCore ROOT::Hist ROOT::Tree ${Geant4_LIBRARIES})
target_compile_options(mc PRIVATE -Wall -Wno-unused-variable -Wno-ignored-qualifiers -Wno-unused-but-set-variable -Wno-shadow -Wno-unused-parameter)

# load common directory
add_subdirectory(../common common)
include_directories(../common)
target_include_directories(mc PUBLIC "${CMAKE_BINARY_DIR}") 


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

install(TARGETS mc DESTINATION bin)

