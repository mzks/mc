cmake_minimum_required(VERSION 3.0)
project(mc)


IF (APPLE)
   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
ENDIF()

add_subdirectory(source)
add_custom_target(alltar DEPENDS source)



