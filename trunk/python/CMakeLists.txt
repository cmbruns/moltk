
include_directories(${CMAKE_SOURCE_DIR}/c++/include)

# Create setup.py for local install and other uses
set(SETUP_PY "${CMAKE_SOURCE_DIR}/setup.py") # in root dir where users expect it
configure_file(setup.py.in ${SETUP_PY} @ONLY)

# Local install of moltk python module for testing
set(MOLTK_LOCAL_INSTALL_DIR "${CMAKE_BINARY_DIR}/py_install")
# setup.py --home argument helps create consistent lib/python destination...
set(MOLTK_LOCAL_PYTHON_PATH ${MOLTK_LOCAL_INSTALL_DIR}/lib/python)
file(TO_NATIVE_PATH "${MOLTK_LOCAL_PYTHON_PATH}" NATIVE_LOCAL_PYTHONPATH)

# create cmake script for running setup.py at build time
# so we can run with env variables of our choosing.
set(RUN_SETUP_PY "${CMAKE_CURRENT_BINARY_DIR}/RunSetupPy.cmake")
file(RELATIVE_PATH BUILD_BASE "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/py_build")
file(RELATIVE_PATH REL_LOCAL_INSTALL_DIR "${CMAKE_SOURCE_DIR}" "${MOLTK_LOCAL_INSTALL_DIR}")
file(WRITE "${RUN_SETUP_PY}"
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
# Build/install after clean
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
set(SETUP_PY_STAMP "${CMAKE_CURRENT_BINARY_DIR}/setup_py.stamp")
add_custom_command(
    OUTPUT "${SETUP_PY_STAMP}"
    COMMAND "${CMAKE_COMMAND}" -P "${RUN_SETUP_PY}"
    COMMAND "${CMAKE_COMMAND}" -E touch "${SETUP_PY_STAMP}"
    DEPENDS 
        "${RUN_SETUP_PY}"
        "${SETUP_PY}"
        ${MOLTK_HEADERS}
        ${MOLTK_SOURCES}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Installing python MolTK to local directory ${MOLTK_LOCAL_INSTALL_DIR}"
)

add_custom_target(LocalPyMolTKInstallation ALL
    DEPENDS 
        "${SETUP_PY_STAMP}"
    SOURCES
        "${SETUP_PY}"
)

add_subdirectory(src)

# local python moltk install symbols required before doc
# (see comments in doc CMakeLists.txt)
add_subdirectory(doc)

add_subdirectory(test)

