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

# python API documentation generated with epydoc
set(PYTHON_API_DIR "${CMAKE_CURRENT_SOURCE_DIR}/api")
file(MAKE_DIRECTORY ${PYTHON_API_DIR}/python)
install(DIRECTORY "${PYTHON_API_DIR}/python"
        DESTINATION "doc/api/python")
# python_api.html is just a shortcut to the epydoc index.html.
install(FILES "python_api.html" DESTINATION "doc")
# Python API documentation generated with epydoc
set(MOLTK_REGENERATE_PYTHON_API_DOCS OFF CACHE BOOL "Whether to (re)generate moltk python API html documents using epydoc")
if(MOLTK_REGENERATE_PYTHON_API_DOCS)
    find_file(EPYDOC_SCRIPT 
        NAMES epydoc.py epydoc
        PATHS
            "${PYTHON_INCLUDE_DIR}/../Scripts"
            /usr/local/bin
            "$ENV{HOME}/bin"
    )
    # We might want to use the program "dot" to generate graphs for the API docs.
    set(DOT_ARG "")
    find_program(DOT_EXECUTABLE dot)
    if(DOT_EXECUTABLE)
        set(DOT_ARG "--graph all")
    endif()
    # Create a little cmake script to generate api docs at build time
    set(DOCSCRIPT "${CMAKE_BINARY_DIR}/generate_epydoc_html.cmake")
    file(WRITE "${DOCSCRIPT}"
    "
        set(ENV{PYTHONPATH} \"${MOLTK_LOCAL_PYTHON_PATH}\")
        execute_process(
        COMMAND \"${PYTHON_EXECUTABLE}\" \"${EPYDOC_SCRIPT}\"
          # -v
          --html moltk -o python
          --name \"MolTK python API v${MOLTK_VERSION} Documentation\"
          --url http://moltk.rotatingpenguin.com/
          --css epydoc_sahara.css
          ${DOT_ARG}
        WORKING_DIRECTORY \"${PYTHON_API_DIR}\")
    ")
    # Run this script after local-install, but before install
    add_custom_target(MoltkPythonApiDocs ALL
        COMMAND ${CMAKE_COMMAND} ARGS -P "${DOCSCRIPT}"
        DEPENDS "${DOCSCRIPT}" ${PURE_PYTHON_SRCS}
        SOURCES "${DOCSCRIPT}")
    add_dependencies(MoltkPythonApiDocs LocalPyMolTKInstallation)
endif()

# TODO
#   LocalPyMolTKInstallation target
#   PURE_PYTHON_SRCS variable
#   NATIVE_LOCAL_PYTHONPATH variable

