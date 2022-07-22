cmake_minimum_required(VERSION 3.13)

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
target_link_libraries(mcProcessing ${ROOT_LIBRARIES})
target_link_libraries(basic_check ${ROOT_LIBRARIES})


install(TARGETS mcProcessing DESTINATION bin)
install(TARGETS basic_check DESTINATION bin)

