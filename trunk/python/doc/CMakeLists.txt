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
          --url http://code.google.com/p/moltk/
        WORKING_DIRECTORY \"${PYTHON_API_DIR}\")
    ")
    # Run this script after local-install, but before install
    add_custom_command(
        OUTPUT "${PYTHON_API_DIR}/python/index.html"
        COMMAND ${CMAKE_COMMAND} ARGS -P "${DOCSCRIPT}"
        COMMAND ${CMAKE_COMMAND} ARGS -E touch "${PYTHON_API_DIR}/python/index.html"
        DEPENDS "${DOCSCRIPT}" InstallMolTKLocally ${PURE_PYTHON_SRCS})
    add_custom_target(MoltkPythonApiDocs ALL
        DEPENDS "${PYTHON_API_DIR}/python/index.html" LocalPyMolTKInstallation ${PURE_PYTHON_SRCS}
        SOURCES "${DOCSCRIPT}")
endif()

# TODO
#   LocalPyMolTKInstallation target
#   PURE_PYTHON_SRCS variable
#   NATIVE_LOCAL_PYTHONPATH variable

