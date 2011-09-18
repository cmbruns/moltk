project(MolTK)
cmake_minimum_required(VERSION 2.8)

include_directories(include)

if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
endif()
set(Boost_ADDITIONAL_VERSIONS 1.47)
find_package(Boost COMPONENTS iostreams zlib REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(src/c++/chem/pdb)

enable_testing()
add_subdirectory(test)
