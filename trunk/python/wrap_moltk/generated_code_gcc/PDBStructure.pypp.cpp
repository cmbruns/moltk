// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "PDBStructure.pypp.hpp"

namespace bp = boost::python;

struct PDBStructure_wrapper : moltk::PDBStructure, bp::wrapper< moltk::PDBStructure > {

    struct Chain_wrapper : moltk::PDBStructure::Chain, bp::wrapper< moltk::PDBStructure::Chain > {
    
        Chain_wrapper( )
        : moltk::PDBStructure::Chain( )
          , bp::wrapper< moltk::PDBStructure::Chain >(){
            // null constructor
        
        }
    
        Chain_wrapper(::moltk::PDBStructure::Chain const & rhs )
        : moltk::PDBStructure::Chain( boost::ref(rhs) )
          , bp::wrapper< moltk::PDBStructure::Chain >(){
            // copy constructor
        
        }
    
        virtual ::size_t get_number_of_residues(  ) const  {
            if( bp::override func_get_number_of_residues = this->get_override( "get_number_of_residues" ) )
                return func_get_number_of_residues(  );
            else{
                return this->moltk::PDBStructure::Chain::get_number_of_residues(  );
            }
        }
        
        ::size_t default_get_number_of_residues(  ) const  {
            return moltk::PDBStructure::Chain::get_number_of_residues( );
        }
    
        virtual void write_to_stream( ::std::ostream & os ) const  {
            if( bp::override func_write_to_stream = this->get_override( "write_to_stream" ) )
                func_write_to_stream( boost::ref(os) );
            else{
                this->moltk::BaseBiosequence::write_to_stream( boost::ref(os) );
            }
        }
        
        void default_write_to_stream( ::std::ostream & os ) const  {
            moltk::BaseBiosequence::write_to_stream( boost::ref(os) );
        }
    
    };

    struct Residue_wrapper : moltk::PDBStructure::Residue, bp::wrapper< moltk::PDBStructure::Residue > {
    
        Residue_wrapper( )
        : moltk::PDBStructure::Residue( )
          , bp::wrapper< moltk::PDBStructure::Residue >(){
            // null constructor
        
        }
    
        Residue_wrapper(::moltk::PDBStructure::Residue const & rhs )
        : moltk::PDBStructure::Residue( boost::ref(rhs) )
          , bp::wrapper< moltk::PDBStructure::Residue >(){
            // copy constructor
        
        }
    
        virtual char get_one_letter_code(  ) const  {
            if( bp::override func_get_one_letter_code = this->get_override( "get_one_letter_code" ) )
                return func_get_one_letter_code(  );
            else{
                return this->moltk::PDBStructure::Residue::get_one_letter_code(  );
            }
        }
        
        char default_get_one_letter_code(  ) const  {
            return moltk::PDBStructure::Residue::get_one_letter_code( );
        }
    
        virtual int get_residue_number(  ) const  {
            if( bp::override func_get_residue_number = this->get_override( "get_residue_number" ) )
                return func_get_residue_number(  );
            else{
                return this->moltk::PDBStructure::Residue::get_residue_number(  );
            }
        }
        
        int default_get_residue_number(  ) const  {
            return moltk::PDBStructure::Residue::get_residue_number( );
        }
    
    };

    PDBStructure_wrapper( )
    : moltk::PDBStructure( )
      , bp::wrapper< moltk::PDBStructure >(){
        // null constructor
    
    }

    PDBStructure_wrapper(::std::istream & arg0 )
    : moltk::PDBStructure( boost::ref(arg0) )
      , bp::wrapper< moltk::PDBStructure >(){
        // constructor
    
    }

    PDBStructure_wrapper(::std::string const & file_name )
    : moltk::PDBStructure( file_name )
      , bp::wrapper< moltk::PDBStructure >(){
        // constructor
    
    }

    PDBStructure_wrapper(::moltk::PDBStructure const & rhs )
    : moltk::PDBStructure( boost::ref(rhs) )
      , bp::wrapper< moltk::PDBStructure >(){
        // copy constructor
    
    }

};

void register_PDBStructure_class(){

    { //::moltk::PDBStructure
        typedef bp::class_< PDBStructure_wrapper > PDBStructure_exposer_t;
        PDBStructure_exposer_t PDBStructure_exposer = PDBStructure_exposer_t( "PDBStructure", "\n PDBStructure represents a macromolecule structure as found in a\n Protein Data Bank file.\n", bp::init< >() );
        bp::scope PDBStructure_scope( PDBStructure_exposer );
        { //::moltk::PDBStructure::Atom
            typedef bp::class_< moltk::PDBStructure::Atom > Atom_exposer_t;
            Atom_exposer_t Atom_exposer = Atom_exposer_t( "Atom", "\n Structure::Atom represents a chemical atom in a molecular structure.\n" );
            bp::scope Atom_scope( Atom_exposer );
            bp::enum_< moltk::PDBStructure::Atom::RecordType>("RecordType")
                .value("RECORD_TYPE_ATOM", moltk::PDBStructure::Atom::RECORD_TYPE_ATOM)
                .value("RECORD_TYPE_HETATM", moltk::PDBStructure::Atom::RECORD_TYPE_HETATM)
                .export_values()
                ;
            { //::moltk::PDBStructure::Atom::get_record_type
            
                typedef ::moltk::PDBStructure::Atom::RecordType ( ::moltk::PDBStructure::Atom::*get_record_type_function_type )(  ) const;
                
                Atom_exposer.def( 
                    "get_record_type"
                    , get_record_type_function_type( &::moltk::PDBStructure::Atom::get_record_type )
                    , "\n Whether this is a standard macromolecule atom type (ATOM) or other (HETATM).\n" );
            
            }
        }
        bp::class_< PDBStructure_wrapper::Chain_wrapper, bp::bases< moltk::BaseBiosequence > >( "Chain", "\n Chain represents a single polymer or molecule in a PDBStructure.\n", bp::init< >("\n Chain represents a single polymer or molecule in a PDBStructure.\n") )    
            .def( bp::init< moltk::PDBStructure::Chain const & >(( bp::arg("rhs") ), "\n Chain represents a single polymer or molecule in a PDBStructure.\n") )    
            .def( 
                "get_number_of_residues"
                , (::size_t ( ::moltk::PDBStructure::Chain::* )(  ) const)(&::moltk::PDBStructure::Chain::get_number_of_residues)
                , (::size_t ( PDBStructure_wrapper::Chain_wrapper::* )(  ) const)(&PDBStructure_wrapper::Chain_wrapper::default_get_number_of_residues) )    
            .def( 
                "get_residue"
                , (::moltk::BaseBiosequence::BaseResidue const & ( ::moltk::PDBStructure::Chain::* )( ::size_t ) const)(&::moltk::PDBStructure::Chain::get_residue)
                , ( bp::arg("index") )
                , bp::return_value_policy< bp::copy_const_reference >() )    
            .def( 
                "write_to_stream"
                , (void ( ::moltk::BaseBiosequence::* )( ::std::ostream & ) const)(&::moltk::BaseBiosequence::write_to_stream)
                , (void ( PDBStructure_wrapper::Chain_wrapper::* )( ::std::ostream & ) const)(&PDBStructure_wrapper::Chain_wrapper::default_write_to_stream)
                , ( bp::arg("os") ) );
        bp::class_< PDBStructure_wrapper::Residue_wrapper, bp::bases< moltk::BaseBiosequence::BaseResidue > >( "Residue", "\n Residue represents a single amino acid or nucleotide residue, or a small molecule,\n in a macromolecule structure.\n", bp::init< >("\n Residue represents a single amino acid or nucleotide residue, or a small molecule,\n in a macromolecule structure.\n") )    
            .def( bp::init< moltk::PDBStructure::Residue const & >(( bp::arg("rhs") ), "\n Residue represents a single amino acid or nucleotide residue, or a small molecule,\n in a macromolecule structure.\n") )    
            .def( 
                "get_one_letter_code"
                , (char ( ::moltk::PDBStructure::Residue::* )(  ) const)(&::moltk::PDBStructure::Residue::get_one_letter_code)
                , (char ( PDBStructure_wrapper::Residue_wrapper::* )(  ) const)(&PDBStructure_wrapper::Residue_wrapper::default_get_one_letter_code) )    
            .def( 
                "get_residue_number"
                , (int ( ::moltk::PDBStructure::Residue::* )(  ) const)(&::moltk::PDBStructure::Residue::get_residue_number)
                , (int ( PDBStructure_wrapper::Residue_wrapper::* )(  ) const)(&PDBStructure_wrapper::Residue_wrapper::default_get_residue_number) );
        PDBStructure_exposer.def( bp::init< std::istream & >(( bp::arg("arg0") )) );
        bp::implicitly_convertible< std::istream &, moltk::PDBStructure >();
        PDBStructure_exposer.def( bp::init< std::string const & >(( bp::arg("file_name") )) );
        bp::implicitly_convertible< std::string const &, moltk::PDBStructure >();
        PDBStructure_exposer.def( bp::init< moltk::PDBStructure const & >(( bp::arg("rhs") )) );
        { //::moltk::PDBStructure::load_file
        
            typedef bool ( ::moltk::PDBStructure::*load_file_function_type )( ::std::string const & ) ;
            
            PDBStructure_exposer.def( 
                "load_file"
                , load_file_function_type( &::moltk::PDBStructure::load_file )
                , ( bp::arg("file_name") ) );
        
        }
        { //::moltk::PDBStructure::load_stream
        
            typedef bool ( ::moltk::PDBStructure::*load_stream_function_type )( ::std::istream & ) ;
            
            PDBStructure_exposer.def( 
                "load_stream"
                , load_stream_function_type( &::moltk::PDBStructure::load_stream )
                , ( bp::arg("arg0") ) );
        
        }
    }

}
