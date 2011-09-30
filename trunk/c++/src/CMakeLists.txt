
add_library(MoltkLibrary
    ${MOLTK_SOURCES}
    ${MOLTK_HEADERS})
set_target_properties(MoltkLibrary PROPERTIES
    OUTPUT_NAME moltk)

