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

include_directories(${CMAKE_SOURCE_DIR}/c++/include)

# Note location of boost python wrapping code
if(MSVC)
    # MSVC uses wrongly lower case letters in includes.
    set(GENERATED_CODE_DIR "generated_code_msvc")
else()
    set(GENERATED_CODE_DIR "generated_code_gcc")
endif()
file(GLOB PYPLUSPLUS_SRCS "src/wrap_moltk/${GENERATED_CODE_DIR}/*.cpp")
file(GLOB PYPLUSPLUS_HEADERS "src/wrap_moltk/${GENERATED_CODE_DIR}/*.hpp")

add_subdirectory(src)

# Create setup.py for local install and other uses
set(SETUP_PY "${CMAKE_SOURCE_DIR}/setup.py") # in root dir where users expect it
configure_file(setup.py.in ${SETUP_PY} @ONLY)

# Local install of moltk python module for testing
set(MOLTK_LOCAL_INSTALL_DIR "${CMAKE_BINARY_DIR}/py_install")
# setup.py --home argument helps create consistent lib/python destination...
set(MOLTK_LOCAL_PYTHON_PATH ${MOLTK_LOCAL_INSTALL_DIR}/lib/python)
file(TO_NATIVE_PATH "${MOLTK_LOCAL_PYTHON_PATH}" NATIVE_LOCAL_PYTHONPATH)

file(RELATIVE_PATH BUILD_BASE "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/py_build")
file(RELATIVE_PATH REL_LOCAL_INSTALL_DIR "${CMAKE_SOURCE_DIR}" "${MOLTK_LOCAL_INSTALL_DIR}")

##################
# setup.py clean #
##################

# create cmake scripts for running setup.py at build time
# so we can run with env variables of our choosing.
set(RUN_SETUP_PY_CLEAN "${CMAKE_CURRENT_BINARY_DIR}/RunSetupPyClean.cmake")
file(WRITE "${RUN_SETUP_PY_CLEAN}"
"
################
set(ENV{BOOST_PYTHON_LIBRARY} \"${Boost_PYTHON_LIBRARY_RELEASE}\")
set(ENV{BOOST_IOSTREAMS_LIBRARY} \"${Boost_IOSTREAMS_LIBRARY_RELEASE}\")
set(ENV{BOOST_INCLUDE_DIR} \"${Boost_INCLUDE_DIRS}\")
# Distutils bug requires "build" command to populate build-base before clean
# Clean first - distutils does not grok source dependencies
execute_process(
    COMMAND \"${PYTHON_EXECUTABLE}\" setup.py
        build --build-base=${BUILD_BASE}
        clean --all
    WORKING_DIRECTORY \"${CMAKE_SOURCE_DIR}\"
)
#################
"
)

# stamp file to keep track of the last time we rebuild the python module
set(SETUP_PY_CLEAN_STAMP "${CMAKE_CURRENT_BINARY_DIR}/setup_py_clean.stamp")
add_custom_command(
    OUTPUT "${SETUP_PY_CLEAN_STAMP}"
    COMMAND "${CMAKE_COMMAND}" -P "${RUN_SETUP_PY_CLEAN}"
    COMMAND "${CMAKE_COMMAND}" -E touch "${SETUP_PY_CLEAN_STAMP}"
    DEPENDS 
        "${RUN_SETUP_PY_CLEAN}"
        "${SETUP_PY}"
        ${MOLTK_HEADERS}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Cleaning python MolTK build files"
)
add_custom_target(SetupPyClean
    DEPENDS 
        "${SETUP_PY_CLEAN_STAMP}"
    SOURCES
        setup.py.in
        "${SETUP_PY}"
)


##################
# setup.py build #
##################

file(GLOB_RECURSE PURE_PYTHON_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/moltk/*.py")
set(RUN_SETUP_PY_BUILD "${CMAKE_CURRENT_BINARY_DIR}/RunSetupPyBuild.cmake")
file(WRITE "${RUN_SETUP_PY_BUILD}"
"
################
set(ENV{BOOST_PYTHON_LIBRARY} \"${Boost_PYTHON_LIBRARY_RELEASE}\")
set(ENV{BOOST_IOSTREAMS_LIBRARY} \"${Boost_IOSTREAMS_LIBRARY_RELEASE}\")
set(ENV{BOOST_INCLUDE_DIR} \"${Boost_INCLUDE_DIRS}\")
execute_process(
    COMMAND \"${PYTHON_EXECUTABLE}\" setup.py
        build --build-base=${BUILD_BASE}
        install --home=${REL_LOCAL_INSTALL_DIR}
    WORKING_DIRECTORY \"${CMAKE_SOURCE_DIR}\"
)
#################
"
)

# stamp file to keep track of the last time we rebuild the python module
set(SETUP_PY_BUILD_STAMP "${CMAKE_CURRENT_BINARY_DIR}/setup_py_build.stamp")
add_custom_command(
    OUTPUT "${SETUP_PY_BUILD_STAMP}"
    COMMAND "${CMAKE_COMMAND}" -P "${RUN_SETUP_PY_BUILD}"
    COMMAND "${CMAKE_COMMAND}" -E touch "${SETUP_PY_BUILD_STAMP}"
    DEPENDS 
        "${RUN_SETUP_PY_BUILD}"
        "${SETUP_PY_CLEAN_STAMP}"
        "${SETUP_PY}"
        ${MOLTK_SOURCES}
        ${PYPLUSPLUS_SRCS}
        ${PYPLUSPLUS_HEADERS}
        ${PURE_PYTHON_SRCS}
        MoltkPyplusplusWrappers
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Building python MolTK module/Installing to local directory ${MOLTK_LOCAL_INSTALL_DIR}"
)
add_custom_target(SetupPyBuild
    DEPENDS 
        "${SETUP_PY_BUILD_STAMP}"
    SOURCES
        setup.py.in
        "${SETUP_PY}"
        ${MOLTK_SOURCES}
        ${MOLTK_HEADERS}
        ${PYPLUSPLUS_SRCS}
        ${PYPLUSPLUS_HEADERS}
)


##################
# setup.py bdist #
##################

file(RELATIVE_PATH DIST_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/py_dist")
set(RUN_SETUP_PY_BDIST "${CMAKE_CURRENT_BINARY_DIR}/RunSetupPyBdist.cmake")
set(BDIST_OPT "bdist")
if(MSVC)
    set(BDIST_OPT "bdist_msi")
endif()
file(WRITE "${RUN_SETUP_PY_BDIST}"
"
################
set(ENV{BOOST_PYTHON_LIBRARY} \"${Boost_PYTHON_LIBRARY_RELEASE}\")
set(ENV{BOOST_IOSTREAMS_LIBRARY} \"${Boost_IOSTREAMS_LIBRARY_RELEASE}\")
set(ENV{BOOST_INCLUDE_DIR} \"${Boost_INCLUDE_DIRS}\")
execute_process(
    COMMAND \"${PYTHON_EXECUTABLE}\" setup.py
        build --build-base=${BUILD_BASE}
        ${BDIST_OPT} --dist-dir=${DIST_DIR}
    WORKING_DIRECTORY \"${CMAKE_SOURCE_DIR}\"
)
#################
"
)

# stamp file to keep track of the last time we rebuild the python module
set(SETUP_PY_BDIST_STAMP "${CMAKE_CURRENT_BINARY_DIR}/setup_py_bdist.stamp")
add_custom_command(
    OUTPUT "${SETUP_PY_BDIST_STAMP}"
    COMMAND "${CMAKE_COMMAND}" -P "${RUN_SETUP_PY_BDIST}"
    COMMAND "${CMAKE_COMMAND}" -E touch "${SETUP_PY_BDIST_STAMP}"
    DEPENDS 
        "${RUN_SETUP_PY_BDIST}"
        "${SETUP_PY_BUILD_STAMP}"
        "${SETUP_PY}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Creating python MolTK binary installer"
)
add_custom_target(SetupPyBdist
    DEPENDS 
        "${SETUP_PY_BDIST_STAMP}"
    SOURCES
        setup.py.in
        "${SETUP_PY}"
)

######################

add_custom_target(LocalPyMolTKInstallation ALL
    DEPENDS 
        SetupPyBuild
    SOURCES
        setup.py.in
        "${SETUP_PY}"
)

# Create gui files
add_subdirectory(moltk/gui)

# local python moltk install symbols required before doc
# (see comments in doc CMakeLists.txt)
add_subdirectory(doc)

add_subdirectory(test)

