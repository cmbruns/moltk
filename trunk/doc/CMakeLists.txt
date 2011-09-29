
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(dfound TRUE)
else()
    set(dfound FALSE)
endif()

set(MOLTK_CREATE_API_DOCS ${dfound} CACHE BOOL "Whether to create moltk C++ API html documents using Doxygen")
if(MOLTK_CREATE_API_DOCS)
    find_package(Doxygen REQUIRED)
    mark_as_advanced(CLEAR DOXYGEN_EXECUTABLE)
    set(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    set(CPP_SOURCE_DIR "${CMAKE_SOURCE_DIR}/include/moltk")
    configure_file(Doxyfile.in ${DOXY_CONFIG} @ONLY )
    add_custom_target(GenerateDoxygenApiDocs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG}
        COMMENT "Generating C++ API html documentation using doxygen"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        SOURCES Doxyfile.in)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html")
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html"
            DESTINATION "doc/api/cxx")
    # This is just a shortcut to the Doxygen index.html.
    install(FILES "cxx_api.html" DESTINATION "doc")
endif()
