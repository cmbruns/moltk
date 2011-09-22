
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS 
    "${CMAKE_SOURCE_DIR}/include/moltk/align.h" 
    "${CMAKE_SOURCE_DIR}/include/moltk/align/*.h")
add_library(moltk_align ${SRCS} ${HEADERS})
