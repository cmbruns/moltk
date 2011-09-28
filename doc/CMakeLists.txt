
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
endif()
