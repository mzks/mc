cmake_minimum_required(VERSION 3.0)
project(mc)
enable_testing()

IF (APPLE)
   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
ENDIF()

add_subdirectory(source)
add_custom_target(alltar DEPENDS source)

add_test(NAME geant4_small_run COMMAND ../bin/mc run.mac)

