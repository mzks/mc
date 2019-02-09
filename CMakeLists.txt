cmake_minimum_required(VERSION 3.0)
project(mc)

# set(CMAKE_CXX_STANDARD 14)

add_subdirectory(source)
add_custom_target(alltar DEPENDS source)



