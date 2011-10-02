// This file has been generated by Py++.

#include "boost/python.hpp"

#include "wrap_moltk.h"

#include "generated_code_msvc/aligner.pypp.hpp"

#include "generated_code_msvc/alignment.pypp.hpp"

#include "generated_code_msvc/bit_unit.pypp.hpp"

#include "generated_code_msvc/fastasequence.pypp.hpp"

#include "generated_code_msvc/information.pypp.hpp"

#include "generated_code_msvc/moltk_global_variables.pypp.hpp"

#include "generated_code_msvc/printable.pypp.hpp"

#include "generated_code_msvc/structure.pypp.hpp"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(moltk){
    register_Aligner_class();

    register_Printable_class();

    register_Alignment_class();

    register_FastaSequence_class();

    register_Structure_class();

    register_Information_class();

    register_bit_unit_class();

    register_global_variables();
}

