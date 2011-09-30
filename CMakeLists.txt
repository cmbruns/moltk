project(MolTK)
cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME MolTK)
set(MolTK_VERSION 0.2.0)

# I think enable_testing() has to be in top-level CMakeLists.txt file.
enable_testing()

# C++ rules must precede python rules,
# because python module depends on C++ libraries.
add_subdirectory(c++)
add_subdirectory(python)
add_subdirectory(doc)

# TODO - packaging

