#    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
#    Copyright (C) 2011  Christopher M. Bruns
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#    
#    For questions contact: cmbruns@rotatingpenguin.com

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
    
    include_dirs = ["./c++/include", 
                    "./python/wrap_moltk",
                    boost_include_dir,
                   ]

    CFLAGS = []
    moltk_sources = glob.glob(os.path.join('c++','src','*.cpp'))
    # instantiate.cpp
    moltk_sources.extend(glob.glob(os.path.join('python', 'wrap_moltk', '*.cpp')))
    
    if (platform.system() == 'Windows'):
        # Avoid compiler warning and subsequent link error with MSVC9 (/EHsc)
        CFLAGS.append('/EHsc')
        # And avoid strange MSVC header errors, to match gccxml args (-D"_HAS_TR1=0")
        CFLAGS.append('-D"_HAS_TR1=0"')
        moltk_sources.extend(glob.glob(os.path.join('python','wrap_moltk','generated_code_msvc','*.cpp')))
        include_dirs.append("./python/wrap_moltk/generated_code_msvc")
    else:
        moltk_sources.extend(glob.glob(os.path.join('python','wrap_moltk','generated_code_gcc','*.cpp')))
        include_dirs.append("./python/wrap_moltk/generated_code_gcc")
    
    library_dirs = [boost_lib_dir,]
    
    setup(name='MolTK',
          version = '0.3.3',
          description = "Python API for moltk sequence/structure alignment/visualization tool kit.",
          author = "Christopher Bruns",
          author_email = "cmbruns@rotatingpenguin.com",
          url = "http://code.google.com/p/moltk/",
          package_dir = {'': 'python'}, # sources are under python subdirectory
          packages = ['moltk', 'moltk.gui'],
          ext_modules = [Extension( 'moltk._moltk', moltk_sources,
                library_dirs = library_dirs,
                libraries = [boost_python_library, boost_iostreams_library],
                include_dirs = include_dirs,
                extra_compile_args = CFLAGS,
                depends = []
                )],
         )

do_setup()

