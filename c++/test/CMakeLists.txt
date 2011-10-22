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

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

########################################################
# Regular automated tests that depend on MolTK library #
########################################################

file(GLOB REGR_TESTS "test*.cpp")
foreach(TEST_PROG ${REGR_TESTS})
    get_filename_component(TEST_NAME_WE ${TEST_PROG} NAME_WE)
    add_executable(${TEST_NAME_WE} ${TEST_PROG})
    set_target_properties(${TEST_NAME_WE} PROPERTIES
        PROJECT_LABEL "Regr - ${TEST_NAME_WE}")
    target_link_libraries(${TEST_NAME_WE}
        ${Boost_IOSTREAMS_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        MoltkLibrary)
    set(TEST_ARGS "")
    if("${TEST_NAME_WE}" STREQUAL "test_load_pdb")
        set(TEST_ARGS "${CMAKE_SOURCE_DIR}/data/structures/1FND.pdb.gz")
    endif()
    add_test(${TEST_NAME_WE} 
        ${TEST_NAME_WE} ${TEST_ARGS})
endforeach()
