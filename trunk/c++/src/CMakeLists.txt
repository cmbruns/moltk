
add_library(MoltkLibrary
    ${MOLTK_SOURCES}
    ${MOLTK_HEADERS})
set_target_properties(MoltkLibrary PROPERTIES
    OUTPUT_NAME moltk)
target_link_libraries(MoltkLibrary
    ${Boost_IOSTREAMS_LIBRARY})

