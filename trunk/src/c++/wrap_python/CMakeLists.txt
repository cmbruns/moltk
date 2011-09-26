find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB PYPLUSPLUS_SRCS "generated_code/*.cpp")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/generated_code")

add_library(moltk_module ${PYPLUSPLUS_SRCS})
set_target_properties(moltk_module PROPERTIES
    OUTPUT_NAME moltk
    PREFIX "")
if(WIN32)
    set_target_properties(moltk_module PROPERTIES
        SUFFIX ".pyd"
        PREFIX "../" # move out of Release/Debug
    )
endif()

set(MOLTK_REGENERATE_PYTHON_BINDINGS OFF CACHE BOOL "Whether to (re)generated C++ code for moltk python bindings")
if(MOLTK_REGENERATE_PYTHON_BINDINGS)
    # Generate bindings with pyplusplus
    set(WRAP_MOLTK_PY ${CMAKE_CURRENT_BINARY_DIR}/wrap_moltk.py)
    find_program(GCCXML_EXECUTABLE # needed by wrap_moltk.py.in
        NAMES gccxml)
    set(MOLTK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include") # needed by wrap_moltk.py.in
    configure_file(wrap_moltk.py.in "${WRAP_MOLTK_PY}")
    find_package(PythonInterp REQUIRED)
    set(MOLTK_GEN_STAMP "${CMAKE_CURRENT_SOURCE_DIR}/generated_code/generate_v3d.stamp")
    add_custom_command(
        COMMAND "${PYTHON_EXECUTABLE}" "${WRAP_MOLTK_PY}"
            > "${CMAKE_CURRENT_BINARY_DIR}/wrap_moltk.log"
            >2 "${CMAKE_CURRENT_BINARY_DIR}/wrap_moltk.err"
        COMMAND ${CMAKE_COMMAND} -E touch "${MOLTK_GEN_STAMP}"
        OUTPUT "${MOLTK_GEN_STAMP}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Creating moltk wrapper source files.  Examine wrap_moltk.err for details."
    )
endif()
