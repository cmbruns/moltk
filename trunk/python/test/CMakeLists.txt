
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(UsePythonTest)

set_source_files_properties(test_align1.py PROPERTIES PYTHONPATH "${MOLTK_LOCAL_PYTHON_PATH}")
ADD_PYTHON_TEST(test_align1_py test_align1.py)

set_source_files_properties(test_end_gaps.py PROPERTIES PYTHONPATH "${MOLTK_LOCAL_PYTHON_PATH}")
ADD_PYTHON_TEST(test_end_gaps_py test_end_gaps.py)
