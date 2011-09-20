
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/include/chem/*.h")
add_library(chempdb ${SRCS} ${HEADERS})
