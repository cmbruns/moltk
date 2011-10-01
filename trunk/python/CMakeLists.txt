
include_directories(${CMAKE_SOURCE_DIR}/c++/include)

# TODO - local python moltk install symbols required before doc
# (see comments in doc CMakeLists.txt)
add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(test)

