find_package(Boost COMPONENTS unit_test_framework REQUIRED)

###############################################################################################
# Create a few custom tests with fewer dependencies while I work through a major refactoring. #
###############################################################################################

add_executable(test_biosequence1 test_biosequence.cpp ../src/Biosequence.cpp)
target_link_libraries(test_biosequence1 ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(test_biosequence1 test_biosequence1)

add_executable(test_vec3a test_vec3.cpp)
target_link_libraries(test_vec3a ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(test_vec3a test_vec3a)

add_executable(test_pdb1 test_pdb.cpp ../src/PdbStructure.cpp ../src/Biosequence.cpp)
target_link_libraries(test_pdb1 
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_IOSTREAMS_LIBRARY}
    ${ZLIB_LIBRARY}
    )
add_test(test_pdb1 test_pdb1)

add_executable(test_alignment1 
    test_alignment.cpp 
    ../src/PdbStructure.cpp 
    ../src/Biosequence.cpp 
    ../src/Alignment.cpp
    )
target_link_libraries(test_alignment1 
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_IOSTREAMS_LIBRARY}
    ${ZLIB_LIBRARY}
    )
add_test(test_alignment1 test_alignment1)

add_executable(test_aligner1 
    test_aligner.cpp 
    ../src/PdbStructure.cpp 
    ../src/Biosequence.cpp 
    ../src/Alignment.cpp
    ../src/Aligner.cpp
    ../src/MatrixScorer.cpp
    )
target_link_libraries(test_aligner1 
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_IOSTREAMS_LIBRARY}
    ${ZLIB_LIBRARY}
    )
add_test(test_aligner1 test_aligner1)

########################################################
# Regular automated tests that depend on MolTK library #
########################################################

file(GLOB REGR_TESTS "test*.cpp")
foreach(TEST_PROG ${REGR_TESTS})
    get_filename_component(TEST_NAME_WE ${TEST_PROG} NAME_WE)
    add_executable(${TEST_NAME_WE} ${TEST_PROG})
    set_target_properties(${TEST_NAME_WE} PROPERTIES
        PROJECT_LABEL "Regr - ${TEST_NAME_WE}")
    target_link_libraries(${TEST_NAME_WE}
        ${Boost_IOSTREAMS_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        MoltkLibrary)
    set(TEST_ARGS "")
    if("${TEST_NAME_WE}" STREQUAL "test_load_pdb")
        set(TEST_ARGS "${CMAKE_SOURCE_DIR}/data/structures/1FND.pdb.gz")
    endif()
    add_test(${TEST_NAME_WE} 
        ${TEST_NAME_WE} ${TEST_ARGS})
endforeach()
