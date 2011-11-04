#    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
#    Copyright (C) 2011  Christopher M. Bruns
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#   
#    Commercial users should ask about our dual licensing model.
#    For questions contact: cmbruns@rotatingpenguin.com

project(MolTK)
# cmake 2.8.3 does not have ADD_TEST(WORKING_DIRECTORY) option
cmake_minimum_required(VERSION 2.8.4)

# Some declarations are common to C++ and python subdirectories
set(MOLTK_VERSION 0.3.6)
add_definitions(-DMOLTK_VERSION="\\"${MOLTK_VERSION}\\"")
include_directories(c++/include)

# I think enable_testing() has to be in top-level CMakeLists.txt file.
enable_testing()

# Default to Release build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Release or Debug" FORCE)
endif()

if(APPLE)
    # Default to Fat 32/64 Build
    if(NOT CMAKE_OSX_ARCHITECTURES)
        set(CMAKE_OSX_ARCHITECTURES "i386;x86_64;ppc" CACHE STRING "" FORCE)
    endif()
    # Default to Leopard support (OSX 10.5)
    if (NOT CMAKE_OSX_DEPLOYMENT_TARGET)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.5" CACHE STRING "Minimum OSX version binaries will run on" FORCE)
    endif()
endif()

# Boost is for both C++ and python
set(Boost_ADDITIONAL_VERSIONS 1.47)
# Link errors with static boost on Fedora
if(UNIX AND NOT APPLE)
    set(Boost_USE_STATIC_LIBS OFF)
    add_definitions("-DBOOST_TEST_DYN_LINK")
else()
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost COMPONENTS iostreams unit_test_framework REQUIRED)
# Boost_LIB_DIAGNOSTIC_DEFINITIONS causes autolink library information to be printed at build time.
# add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
if(WIN32)
    find_package(Boost COMPONENTS zlib REQUIRED)
else()
    find_package(ZLIB REQUIRED)
endif()
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
mark_as_advanced(Boost_DIR)

file(GLOB_RECURSE MOLTK_HEADERS "c++/include/*.hpp")
file(GLOB_RECURSE MOLTK_SOURCES "c++/src/*.cpp")

# C++ rules must precede python rules,
# because python module depends on C++ libraries.
add_subdirectory(c++)

set(MOLTK_BUILD_PYTHON_MODULE ON CACHE BOOL "Whether to create moltk python module")
if(MOLTK_BUILD_PYTHON_MODULE)
    # Want shared library for boost python
    set(Boost_USE_STATIC_LIBS OFF)
    find_package(Boost COMPONENTS python REQUIRED)
    add_subdirectory(python)
endif()

add_subdirectory(doc)

# TODO - packaging

