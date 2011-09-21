
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/include/pdb/*.h")
add_library(moltk_pdb ${SRCS} ${HEADERS})

