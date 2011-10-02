# file: setup.py / setup.py.in
# Used to install or build moltk python module "pythonically".
# ("setup.py.in" is used by CMake to generate the final "setup.py" file.)
# The following environment variables should be set before running this script:
#  BOOST_PYTHON_LIBRARY
#  BOOST_INCLUDE_DIR

from distutils.core import setup
from distutils.extension import Extension
import platform
import glob
import os

boost_include_dir = os.environ.get("BOOST_INCLUDE_DIR")
if not boost_include_dir or not os.path.exists(boost_include_dir):
    print "BOOST_INCLUDE_DIR folder '%s' does not exist.  Set the BOOST_INCLUDE_DIR environment variable to the correct location of the boost header files." \
        % (boost_include_dir)
    raise

boost_python_library_path = os.environ.get("BOOST_PYTHON_LIBRARY")
if not boost_python_library_path or not os.path.exists(boost_python_library_path):
    print "BOOST_PYTHON_LIBRARY file '%s' does not exist.  Set the BOOST_PYTHON_LIBRARY environment variable to the full path of the boost python library file." \
        % (boost_python_library_path)
    raise
boost_lib_dir, boost_python_library = os.path.split(boost_python_library_path)
# remove library filename extension
boost_python_library = os.path.splitext(boost_python_library)[0]
# boost_python_library should be boost_python-vc90-mt-gd-1_47.lib or similar on windows

CFLAGS = []
moltk_sources = glob.glob(os.path.join('c++','src','*.cpp'))

if (platform.system() == 'Windows'):
    # Avoid compiler warning and subsequent link error with MSVC9 (/EHsc)
    CFLAGS.append('/EHsc')
    # And avoid strange MSVC header errors, to match gccxml args (-D"_HAS_TR1=0")
    CFLAGS.append('-D"_HAS_TR1=0"')
    moltk_sources.extend(glob.glob(os.path.join('python','src','wrap_moltk','generated_code_msvc','*.cpp')))
else:
    moltk_sources.extend(glob.glob(os.path.join('python','src','wrap_moltk','generated_code_gcc','*.cpp')))

include_dirs = ["./c++/include", 
                "./python/src/wrap_moltk",
                boost_include_dir,
               ]

library_dirs = [boost_lib_dir,]

setup(name='MolTK',
      version = '0.3.0',
      description = "Python API for moltk sequence/structure alignment/visualization tool kit.",
      author = "Christopher Bruns",
      author_email = "cmbruns@rotatingpenguin.com",
      url = "http://code.google.com/p/moltk/",
      ext_modules = [Extension( 'moltk', moltk_sources,
            library_dirs = library_dirs,
            libraries = [boost_python_library,],
            include_dirs = include_dirs,
            extra_compile_args = CFLAGS,
            depends = []
            )],
     )
