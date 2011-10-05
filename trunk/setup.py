# file: setup.py / setup.py.in
# Used to install or build moltk python module "pythonically".
# ("setup.py.in" is used by CMake to generate the final "setup.py" file.)
# The following environment variables should be set before running this script:
#  BOOST_PYTHON_LIBRARY
#  BOOST_IOSTREAMS_LIBRARY
#  BOOST_INCLUDE_DIR

from distutils.core import setup
from distutils.extension import Extension
import platform
import glob
import os
import re

# Avoid compiler warning about "-Wstrict-prototypes" not being a C++ flag.
import distutils.sysconfig
v = distutils.sysconfig.get_config_vars()
for key in ['PY_CFLAGS', 'CFLAGS', 'OPT']:
    if v.has_key(key):
        v[key] = v[key].replace(' -Wstrict-prototypes', '')

def get_lib_from_env_var(env_string):
    path = os.environ.get(env_string)
    if not path or not os.path.exists(path):
        print "%s file '%s' does not exist.  Set the %s environment variable to the full path of the library file." \
            % (env_string, path, env_string)
        raise
    dir, lib = os.path.split(path)
    # remove library filename extension
    lib = os.path.splitext(lib)[0]
    if (platform.system() != "Windows"):
        # Strip leading "lib" from library name on Mac and Linux
        lib = re.sub(r'^lib', '', lib)
    return (dir, lib)

def do_setup():
    boost_include_dir = os.environ.get("BOOST_INCLUDE_DIR")
    if not boost_include_dir or not os.path.exists(boost_include_dir):
        print "BOOST_INCLUDE_DIR folder '%s' does not exist.  Set the BOOST_INCLUDE_DIR environment variable to the correct location of the boost header files." \
            % (boost_include_dir)
        raise

    boost_lib_dir, boost_python_library = get_lib_from_env_var("BOOST_PYTHON_LIBRARY")
    boost_iostreams_library = get_lib_from_env_var("BOOST_IOSTREAMS_LIBRARY")[1]
    
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
                libraries = [boost_python_library, boost_iostreams_library],
                include_dirs = include_dirs,
                extra_compile_args = CFLAGS,
                depends = []
                )],
         )

do_setup()

