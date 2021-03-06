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

# Set up build environment for moltk binary python extension module

include_directories(${CMAKE_SOURCE_DIR}/c++/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # To find headers under generated code

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_CODE_DIR}")

# (optionally) regenerate python binding source code
# TODO - consider doing this at configure time, with EXEC_PROGRAM()

set(MOLTK_PYTHON_REGENERATE_BINDINGS OFF CACHE BOOL "Whether to (re)generated C++ code for moltk python bindings")
if(MOLTK_PYTHON_REGENERATE_BINDINGS)
    # Generate bindings with pyplusplus
    set(WRAP_MOLTK_PY ${CMAKE_CURRENT_BINARY_DIR}/wrap_moltk.py)
    find_program(GCCXML_EXECUTABLE # needed by wrap_moltk.py.in
        NAMES gccxml)
    set(MOLTK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/c++/include") # needed by wrap_moltk.py.in
    set(MOLTK_WRAP_INCLUDE_FILES wrap_moltk.hpp)
    configure_file(wrap_moltk.py.in "${WRAP_MOLTK_PY}" @ONLY)
    configure_file(doxygen_doc_extractor.py
        ${CMAKE_CURRENT_BINARY_DIR}/doxygen_doc_extractor.py
        COPYONLY)
    find_package(PythonInterp REQUIRED)
    set(MOLTK_GEN_STAMP "${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_CODE_DIR}/generate_moltk.stamp")
    file(GLOB_RECURSE MOLTK_HEADERS "${CMAKE_SOURCE_DIR}/include/*.hpp")
    add_custom_command(
        COMMAND "${PYTHON_EXECUTABLE}" "${WRAP_MOLTK_PY}"
            > "${CMAKE_CURRENT_BINARY_DIR}/wrap_moltk.log"
            2> "${CMAKE_CURRENT_BINARY_DIR}/wrap_moltk.err"
        COMMAND ${CMAKE_COMMAND} -E touch "${MOLTK_GEN_STAMP}"
        OUTPUT 
            "${MOLTK_GEN_STAMP}"
            ${PYPLUSPLUS_SRCS}
            ${PYPLUSPLUS_HEADERS}
        DEPENDS
            "${WRAP_MOLTK_PY}"
            doxygen_doc_extractor.py
            ${MOLTK_HEADERS}
            wrap_moltk.hpp
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Creating moltk wrapper source files.  Examine wrap_moltk.err for details."
    )
    add_custom_target(MoltkPyplusplusWrappers ALL
        DEPENDS 
            "${MOLTK_GEN_STAMP}"
        SOURCES
            wrap_moltk.py.in
            doxygen_doc_extractor.py
            wrap_moltk.hpp
        COMMENT "Generating python wrapper source code"
    )
endif()

