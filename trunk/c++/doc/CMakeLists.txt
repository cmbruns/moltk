# This file, doc/CMakeLists.txt, is used to help build the documenation for the moltk project.

# C++ API documentation generated with doxygen
set(DOXYGEN_API_DIR "${CMAKE_CURRENT_SOURCE_DIR}/api/cxx")
file(MAKE_DIRECTORY ${DOXYGEN_API_DIR}/html)
install(DIRECTORY "${DOXYGEN_API_DIR}/html"
        DESTINATION "doc/api/cxx/html")
# cxx_api.html is just a shortcut to the Doxygen index.html.
install(FILES "cxx_api.html" DESTINATION "doc")

set(MOLTK_REGENERATE_CPP_API_DOCS OFF CACHE BOOL "Whether to (re)generate moltk C++ API html documents using Doxygen")
if(MOLTK_REGENERATE_CPP_API_DOCS)
    # Doxygen is a program for generating API documentation files from source code comments.
    find_package(Doxygen REQUIRED)
    mark_as_advanced(CLEAR DOXYGEN_EXECUTABLE)
    set(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    set(CPP_SOURCE_DIR "${CMAKE_SOURCE_DIR}/include/moltk/pdb/Atom.h")
    configure_file(Doxyfile.in ${DOXY_CONFIG} @ONLY )
    add_custom_target(GenerateDoxygenApiDocs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG}
        COMMENT "Generating C++ API html documentation using doxygen"
        WORKING_DIRECTORY ${DOXYGEN_API_DIR}
        SOURCES Doxyfile.in)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html")
endif()

# python API documentation generated with epydoc
set(PYTHON_API_DIR "${CMAKE_CURRENT_SOURCE_DIR}/api/python")
file(MAKE_DIRECTORY ${PYTHON_API_DIR}/api/python/doc)
install(DIRECTORY "${PYTHON_API_DIR}/api/python/doc"
        DESTINATION "doc/api/python/doc")
# python_api.html is just a shortcut to the Doxygen index.html.
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
        set(ENV{PYTHONPATH} \"${NATIVE_LOCAL_PYTHONPATH}\")
        execute_process(
        COMMAND \"${PYTHON_EXECUTABLE}\" \"${EPYDOC_SCRIPT}\"
          --html -o doc moltk
          --name \"MolTK API v${MOLTK_VERSION} Documentation\"
          --url http://code.google.com/p/moltk/
        WORKING_DIRECTORY \"${PYTHON_API_DIR}\")
    ")
    # Run this script after local-install, but before install
    add_custom_command(
        OUTPUT "${PYTHON_API_DIR}/doc/index.html"
        COMMAND ${CMAKE_COMMAND} ARGS -P "${DOCSCRIPT}"
        COMMAND ${CMAKE_COMMAND} ARGS -E touch "${PYTHON_API_DIR}/doc/index.html"
        DEPENDS "${DOCSCRIPT}" InstallMolTKLocally ${PURE_PYTHON_SRCS})
    add_custom_target(GeneratePythonApiDocs ALL
        DEPENDS "${PYTHON_API_DIR}/doc/index.html" InstallMolTKLocally ${PURE_PYTHON_SRCS}
        SOURCES "${DOCSCRIPT}")
endif()

# TODO
#   InstallMolTKLocally target
#   PURE_PYTHON_SRCS variable
#   NATIVE_LOCAL_PYTHONPATH variable
