// This file has been generated by Py++.

#include "boost/python.hpp"

#include "indexing_suite/container_suite.hpp"

#include "indexing_suite/vector.hpp"

#include "wrap_moltk.hpp"

#include "generated_code_msvc/_moltk_enumerations.pypp.hpp"

#include "generated_code_msvc/_moltk_free_functions.pypp.hpp"

#include "generated_code_msvc/_moltk_global_variables.pypp.hpp"

#include "generated_code_msvc/aligner.pypp.hpp"

#include "generated_code_msvc/alignment.pypp.hpp"

#include "generated_code_msvc/angledimension.pypp.hpp"

#include "generated_code_msvc/basebiosequence.pypp.hpp"

#include "generated_code_msvc/biosequence.pypp.hpp"

#include "generated_code_msvc/biosequenceresidue.pypp.hpp"

#include "generated_code_msvc/bitunit.pypp.hpp"

#include "generated_code_msvc/displacement.pypp.hpp"

#include "generated_code_msvc/information.pypp.hpp"

#include "generated_code_msvc/informationdimension.pypp.hpp"

#include "generated_code_msvc/length.pypp.hpp"

#include "generated_code_msvc/lengthdimension.pypp.hpp"

#include "generated_code_msvc/nanometerunit.pypp.hpp"

#include "generated_code_msvc/pdbstructure.pypp.hpp"

#include "generated_code_msvc/printable.pypp.hpp"

#include "generated_code_msvc/quaternion.pypp.hpp"

#include "generated_code_msvc/querypositionlist.pypp.hpp"

#include "generated_code_msvc/radianunit.pypp.hpp"

#include "generated_code_msvc/rigidtransform.pypp.hpp"

#include "generated_code_msvc/rotation3d.pypp.hpp"

#include "generated_code_msvc/std_vector_int.pypp.hpp"

#include "generated_code_msvc/targetpositionlist.pypp.hpp"

#include "generated_code_msvc/unitvector3d.pypp.hpp"

#include "generated_code_msvc/vector3d.pypp.hpp"

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

    register_Quaternion_class();

    register_RigidTransform_class();

    register_Rotation3D_class();

    register_UnitVector3D_class();

    register_Vector3D_class();

    register_Displacement_class();

    register_InformationDimension_class();

    register_AngleDimension_class();

    register_LengthDimension_class();

    register_Information_class();

    register_Length_class();

    register_BitUnit_class();

    register_RadianUnit_class();

    register_NanometerUnit_class();

    register_global_variables();

    register_free_functions();
}

