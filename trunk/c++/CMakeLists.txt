
include_directories(include)

set(Boost_ADDITIONAL_VERSIONS 1.47)
if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost COMPONENTS iostreams zlib REQUIRED)
else()
    find_package(Boost COMPONENTS iostreams REQUIRED)
endif()
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)

