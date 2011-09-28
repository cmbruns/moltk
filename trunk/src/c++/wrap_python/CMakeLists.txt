
# Set up build environment for moltk binary python extension module

include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # To find headers under generated code

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

if(MSVC)
    # MSVC uses wrongly lower case letters in includes.
    set(GENERATED_CODE_DIR "generated_code_msvc")
else()
    set(GENERATED_CODE_DIR "generated_code_gcc")
endif()
file(GLOB PYPLUSPLUS_SRCS "${GENERATED_CODE_DIR}/*.cpp")
file(GLOB PYPLUSPLUS_HEADERS "${GENERATED_CODE_DIR}/*.hpp")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_CODE_DIR}")

add_library(moltk_module SHARED ${PYPLUSPLUS_SRCS} ${PYPLUSPLUS_HEADERS})
target_link_libraries(moltk_module
    ${PYTHON_LIBRARY})
set_target_properties(moltk_module PROPERTIES
    OUTPUT_NAME moltk
    PREFIX "")
if(WIN32)
    set_target_properties(moltk_module PROPERTIES
        SUFFIX ".pyd"
        PREFIX "../" # move out of Release/Debug
    )
else()
    # Windows seems to auto-link boost-python, and has trouble with explicit link. So only explicitly link on Mac/Linux
    target_link_libraries(moltk_module
        ${Boost_PYTHON_LIBRARY}
        moltk_pdb)
    set_target_properties(moltk_module PROPERTIES
        SUFFIX ".so" # even on Mac! (i.e. not .dylib)
    )
endif()


# (optionally) regenerate python binding source code
# TODO - consider doing this at configure time, with EXEC_PROGRAM()

set(MOLTK_REGENERATE_PYTHON_BINDINGS OFF CACHE BOOL "Whether to (re)generated C++ code for moltk python bindings")
if(MOLTK_REGENERATE_PYTHON_BINDINGS)
    # Generate bindings with pyplusplus
    set(WRAP_MOLTK_PY ${CMAKE_CURRENT_BINARY_DIR}/wrap_moltk.py)
    find_program(GCCXML_EXECUTABLE # needed by wrap_moltk.py.in
        NAMES gccxml)
    set(MOLTK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include") # needed by wrap_moltk.py.in
    set(MOLTK_WRAP_INCLUDE_FILES "moltk/pdb/Atom.h")
    configure_file(wrap_moltk.py.in "${WRAP_MOLTK_PY}" @ONLY)
    configure_file(doxygen_doc_extractor.py
        ${CMAKE_CURRENT_BINARY_DIR}/doxygen_doc_extractor.py
        COPYONLY)
    find_package(PythonInterp REQUIRED)
    set(MOLTK_GEN_STAMP "${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_CODE_DIR}/generate_moltk.stamp")
    file(GLOB_RECURSE MOLTK_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
    add_custom_command(
        COMMAND "${PYTHON_EXECUTABLE}" "${WRAP_MOLTK_PY}"
            > "${CMAKE_CURRENT_BINARY_DIR}/wrap_moltk.log"
            2> "${CMAKE_CURRENT_BINARY_DIR}/wrap_moltk.err"
        COMMAND ${CMAKE_COMMAND} -E touch "${MOLTK_GEN_STAMP}"
        OUTPUT "${MOLTK_GEN_STAMP}"
        DEPENDS
            "${WRAP_MOLTK_PY}"
            doxygen_doc_extractor.py
            ${MOLTK_HEADERS}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Creating moltk wrapper source files.  Examine wrap_moltk.err for details."
    )
    add_custom_target(GenerateWrapperSources
        DEPENDS "${MOLTK_GEN_STAMP}"
        SOURCES
            wrap_moltk.py.in
            doxygen_doc_extractor.py
        COMMENT "Generating python wrapper source code"
    )
    add_dependencies(moltk_module GenerateWrapperSources)
endif()

