# This file, doc/CMakeLists.txt, is used to help build the documenation for the moltk project.

# If you use a glob, exclude CMakeLists.txt for installation
set(DOC_FILES Install.txt)
install(FILES ${DOC_FILES} DESTINATION doc)

