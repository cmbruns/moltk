project(MolTK)
cmake_minimum_required(VERSION 2.8)

include_directories(include)

set(Boost_ADDITIONAL_VERSIONS 1.47)
find_package(Boost COMPONENTS iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

enable_testing()
add_subdirectory(test)
