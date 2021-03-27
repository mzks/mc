cmake_minimum_required(VERSION 3.8)

project(mc)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
include_directories(${PROJECT_SOURCE_DIR}/include)
set(CMAKE_INSTALL_PREFIX ..)

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

install(TARGETS mc DESTINATION bin)
