
add_executable(test_load_pdb test_load_pdb.cpp)
target_link_libraries(test_load_pdb 
    ${Boost_IOSTREAMS_LIBRARY}
    chempdb)
add_test(test_load_pdb 
    test_load_pdb
    "${CMAKE_CURRENT_SOURCE_DIR}/structures/1FND.pdb.gz")

