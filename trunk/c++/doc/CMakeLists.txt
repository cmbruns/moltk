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

# This file, doc/CMakeLists.txt, is used to help build the documenation for the moltk project.

# C++ API documentation generated with doxygen
set(DOXYGEN_API_DIR "${CMAKE_CURRENT_SOURCE_DIR}/api")
file(MAKE_DIRECTORY ${DOXYGEN_API_DIR}/cxx)
install(DIRECTORY "${DOXYGEN_API_DIR}/cxx"
        DESTINATION "doc/api/cxx")
# cxx_api.html is just a shortcut to the Doxygen index.html.
install(FILES "cxx_api.html" DESTINATION "doc")

set(MOLTK_DOC_API_REGENERATE_CPP OFF CACHE BOOL "Whether to (re)generate moltk C++ API html documents using Doxygen")
if(MOLTK_DOC_API_REGENERATE_CPP)
    # Doxygen is a program for generating API documentation files from source code comments.
    find_package(Doxygen REQUIRED)
    mark_as_advanced(CLEAR DOXYGEN_EXECUTABLE)
    mark_as_advanced(CLEAR DOXYGEN_DOT_EXECUTABLE)
    set(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    set(CPP_SOURCE_DIR "${CMAKE_SOURCE_DIR}/c++/include")
    set(MOLTK_DOXYGEN_LOGO "${CMAKE_SOURCE_DIR}/doc/moltk55.png")
    set(DOXYGEN_STYLE_SHEET "${CMAKE_CURRENT_SOURCE_DIR}/api/doxygen_sahara.css")
    set(HAVE_DOT NO)
    if(DOXYGEN_DOT_FOUND)
        set(HAVE_DOT YES)
    endif()
    configure_file(Doxyfile.in ${DOXY_CONFIG} @ONLY )
    # Remove previous documentation files before generating new ones
    file(GLOB OLD_DOCS 
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/api" 
        "api/cxx/*.html" "api/cxx/*.png" "api/cxx/*.md5")
    # message("${OLD_DOCS}")
    string(REPLACE ";" " " OLD_DOCS "${OLD_DOCS}")
    # message("${OLD_DOCS}")
    add_custom_target(MoltkCxxApiDocs ALL
        COMMAND ${CMAKE_COMMAND} -E remove "${OLD_DOCS}"
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG}
        COMMAND ${CMAKE_COMMAND} -E copy tabs.css cxx/tabs.css
        COMMAND ${CMAKE_COMMAND} -E copy doxygen.css cxx/doxygen.css
        COMMENT "Generating C++ API html documentation using doxygen"
        WORKING_DIRECTORY ${DOXYGEN_API_DIR}
        SOURCES Doxyfile.in)
endif()

