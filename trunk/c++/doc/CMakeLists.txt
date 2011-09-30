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
endif()

