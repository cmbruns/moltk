
add_executable(test_load_pdb test_load_pdb.cpp)
target_link_libraries(test_load_pdb 
    ${Boost_IOSTREAMS_LIBRARY}
    MoltkLibrary)
add_test(test_load_pdb 
    test_load_pdb
    "${CMAKE_SOURCE_DIR}/data/structures/1FND.pdb.gz")

add_executable(test_align test_align.cpp)
target_link_libraries(test_align 
    MoltkLibrary)
add_test(test_align test_align)
