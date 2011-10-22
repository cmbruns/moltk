// This file has been generated by Py++.

#include "boost/python.hpp"

#include "indexing_suite/container_suite.hpp"

#include "indexing_suite/vector.hpp"

#include "wrap_moltk.hpp"

#include "generated_code_gcc/Aligner.pypp.hpp"

#include "generated_code_gcc/Alignment.pypp.hpp"

#include "generated_code_gcc/BaseBiosequence.pypp.hpp"

#include "generated_code_gcc/Biosequence.pypp.hpp"

#include "generated_code_gcc/BiosequenceResidue.pypp.hpp"

#include "generated_code_gcc/BitUnit.pypp.hpp"

#include "generated_code_gcc/Displacement.pypp.hpp"

#include "generated_code_gcc/Information.pypp.hpp"

#include "generated_code_gcc/InformationDimension.pypp.hpp"

#include "generated_code_gcc/Length.pypp.hpp"

#include "generated_code_gcc/LengthDimension.pypp.hpp"

#include "generated_code_gcc/NanometerUnit.pypp.hpp"

#include "generated_code_gcc/PDBStructure.pypp.hpp"

#include "generated_code_gcc/Printable.pypp.hpp"

#include "generated_code_gcc/QueryPositionList.pypp.hpp"

#include "generated_code_gcc/RigidTransform.pypp.hpp"

#include "generated_code_gcc/Rotation3D.pypp.hpp"

#include "generated_code_gcc/TargetPositionList.pypp.hpp"

#include "generated_code_gcc/UnitVector3D.pypp.hpp"

#include "generated_code_gcc/Vector3D.pypp.hpp"

#include "generated_code_gcc/_moltk_enumerations.pypp.hpp"

#include "generated_code_gcc/_moltk_free_functions.pypp.hpp"

#include "generated_code_gcc/_moltk_global_variables.pypp.hpp"

#include "generated_code_gcc/std_vector_int.pypp.hpp"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(_moltk){
    register_enumerations();

    register_TargetPositionList_class();

    register_QueryPositionList_class();

    register_std_vector_int_class();

    register_Aligner_class();

    register_Alignment_class();

    register_BaseBiosequence_class();

    register_Biosequence_class();

    register_BiosequenceResidue_class();

    register_PDBStructure_class();

    register_Printable_class();

    register_RigidTransform_class();

    register_Rotation3D_class();

    register_UnitVector3D_class();

    register_Vector3D_class();

    register_Displacement_class();

    register_InformationDimension_class();

    register_LengthDimension_class();

    register_Information_class();

    register_Length_class();

    register_BitUnit_class();

    register_NanometerUnit_class();

    register_global_variables();

    register_free_functions();
}
