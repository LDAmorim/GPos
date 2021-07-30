project(GPos)

set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "./install")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

find_package(MPI REQUIRED)
find_package(Geant4 REQUIRED)
find_package(G4mpi REQUIRED)
find_package(openPMD CONFIG REQUIRED)

include(${Geant4_USE_FILE})
include_directories(${Geant4_INCLUDE_DIR} ${G4mpi_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(GPos src/GPos.cc ${sources} ${headers})

target_link_libraries(GPos PRIVATE ${G4mpi_LIBRARIES})
target_link_libraries(GPos PRIVATE ${Geant4_LIBRARIES})
target_link_libraries(GPos PRIVATE openPMD::openPMD)

install(TARGETS GPos DESTINATION bin)

