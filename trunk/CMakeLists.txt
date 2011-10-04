project(MolTK)
cmake_minimum_required(VERSION 2.8)

# I think enable_testing() has to be in top-level CMakeLists.txt file.
enable_testing()

# Some declarations are common to C++ and python subdirectories
set(MOLTK_VERSION 0.3.0)
add_definitions(-DMOLTK_VERSION="\\"${MOLTK_VERSION}\\"")
include_directories(c++/include)

# Boost is for both C++ and python
set(Boost_ADDITIONAL_VERSIONS 1.47)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS iostreams REQUIRED)
add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
if(WIN32)
    find_package(Boost COMPONENTS zlib REQUIRED)
else()
    find_package(ZLIB REQUIRED)
endif()
# Want shared library for boost python
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# C++ rules must precede python rules,
# because python module depends on C++ libraries.
add_subdirectory(c++)
add_subdirectory(python)
add_subdirectory(doc)

# TODO - packaging

