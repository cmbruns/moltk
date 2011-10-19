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
#    For questions contact: cmbruns@rotatingpenguin.com

project(MolTK)
cmake_minimum_required(VERSION 2.8)

# I think enable_testing() has to be in top-level CMakeLists.txt file.
enable_testing()

# Some declarations are common to C++ and python subdirectories
set(MOLTK_VERSION 0.3.1)
add_definitions(-DMOLTK_VERSION="\\"${MOLTK_VERSION}\\"")
include_directories(c++/include)

# Boost is for both C++ and python
set(Boost_ADDITIONAL_VERSIONS 1.47)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS iostreams REQUIRED)
# Boost_LIB_DIAGNOSTIC_DEFINITIONS causes autolink library information to be printed at build time.
# add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
if(WIN32)
    find_package(Boost COMPONENTS zlib REQUIRED)
else()
    find_package(ZLIB REQUIRED)
endif()
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

file(GLOB_RECURSE MOLTK_HEADERS "c++/include/*.h")
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

