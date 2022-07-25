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
add_test(NAME small_mc COMMAND ../bin/mc -p ../bench -m run.mac -o mc_small.root -r) # store commands in ROOT
add_test(NAME small_mc2 COMMAND ../bin/mc -p ../bench -m run.mac -o mc_small2.root -k) # store commands as ascii macro
add_test(NAME reproducibility COMMAND ../bin/reproducibility -i mc_small.root -j mc_small2.root -t tree) # both tree is the same
add_test(NAME dump1 COMMAND sh -c "../bin/dump.sh mc_small.root >> dump1.txt")
add_test(NAME dump2 COMMAND sh -c "../bin/dump.sh mc_small2.root >> dump2.txt")
add_test(NAME dump_tree_diff COMMAND diff dump1.txt dump2.txt)
add_test(NAME batch_mc COMMAND ../bin/mc -p ../bench -m bat.mac -o mc.root -k) # a bit more production
add_test(NAME exists_macro COMMAND ls ./mc_macro.txt) # generated macro exists
add_test(NAME run_batch_again COMMAND ../bin/mc -p ../bench -m mc_macro.txt -o mc_repro.root -k -r) # run with generated macro
add_test(NAME reproducibility_macro COMMAND ../bin/reproducibility -i mc.root -j mc_repro.root -t tree) # check again
add_test(NAME mcProcessing COMMAND ../bin/mcProcessing -i mc.root -o processed.root --clone) # Processing
add_test(NAME basic_check COMMAND ../bin/basic_check -i processed.root) # basic check for processing tree
