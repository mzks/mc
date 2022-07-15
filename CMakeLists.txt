cmake_minimum_required(VERSION 3.0)
enable_testing()

project(MC)

IF (APPLE)
   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
ENDIF()

add_subdirectory(mc)
add_custom_target(alltar DEPENDS source)

