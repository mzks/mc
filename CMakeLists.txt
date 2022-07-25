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

add_subdirectory(mc)
add_subdirectory(analysis)
# add your project here

add_custom_target(alltar DEPENDS mc analysis)

## Tests
add_test(NAME small_mc COMMAND ../bin/mc -p ../bench -m run.mac -o mc_small.root)
add_test(NAME batch_mc COMMAND ../bin/mc -p ../bench -m bat.mac -o mc.root)
add_test(NAME mcProcessing COMMAND ../bin/mcProcessing -i mc.root -o processed.root --clone)
add_test(NAME basic_check COMMAND ../bin/basic_check -i processed.root)
