// This file has been generated by Py++.

#include "boost/python.hpp"
#include "moltk/pdb/atom.h"
#include "atom.pypp.hpp"

namespace bp = boost::python;

void register_Atom_class(){

    { //::moltk::pdb::Atom
        typedef bp::class_< moltk::pdb::Atom > Atom_exposer_t;
        Atom_exposer_t Atom_exposer = Atom_exposer_t( "Atom", "\n Atom represents a chemical atom in a molecular structure.\n" );
        bp::scope Atom_scope( Atom_exposer );
        bp::enum_< moltk::pdb::Atom::RecordType>("RecordType")
            .value("ATOM_RECORD_TYPE", moltk::pdb::Atom::ATOM_RECORD_TYPE)
            .value("HETATM_RECORD_TYPE", moltk::pdb::Atom::HETATM_RECORD_TYPE)
            .export_values()
            ;
        { //::moltk::pdb::Atom::foo
        
            typedef int ( ::moltk::pdb::Atom::*foo_function_type )(  ) const;
            
            Atom_exposer.def( 
                "foo"
                , foo_function_type( &::moltk::pdb::Atom::foo )
                , "\n Test function to test python bindings.\n" );
        
        }
        { //::moltk::pdb::Atom::getRecordType
        
            typedef ::moltk::pdb::Atom::RecordType ( ::moltk::pdb::Atom::*getRecordType_function_type )(  ) const;
            
            Atom_exposer.def( 
                "getRecordType"
                , getRecordType_function_type( &::moltk::pdb::Atom::getRecordType )
                , "\n Whether this is a standard macromolecule atom type (ATOM) or other (HETATM).\n" );
        
        }
    }

}
