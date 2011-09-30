# This file, doc/CMakeLists.txt, is used to help build the documenation for the moltk project.

file(GLOB DOC_FILES "*.txt")
install(FILES ${DOC_FILES} DESTINATION doc)

