find_package(Boost COMPONENTS unit_test_framework REQUIRED)

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
