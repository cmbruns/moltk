// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "printable.pypp.hpp"

namespace bp = boost::python;

struct Printable_wrapper : moltk::Printable, bp::wrapper< moltk::Printable > {

    Printable_wrapper()
    : moltk::Printable()
      , bp::wrapper< moltk::Printable >(){
        // null constructor
        
    }

    virtual void write_to_stream( ::std::ostream & output_stream ) const {
        bp::override func_write_to_stream = this->get_override( "write_to_stream" );
        func_write_to_stream( boost::ref(output_stream) );
    }

};

void register_Printable_class(){

    bp::class_< Printable_wrapper, boost::noncopyable >( "Printable", "\n moltk::Printable is an abstract base class to help streamline\n string conversion in the moltk python bindings.\n" )    
        .def( 
            "write_to_stream"
            , bp::pure_virtual( (void ( ::moltk::Printable::* )( ::std::ostream & ) const)(&::moltk::Printable::write_to_stream) )
            , ( bp::arg("output_stream") )
            , "\n moltk::Printable is an abstract base class to help streamline\n string conversion in the moltk python bindings.\n" )    
        .def( bp::self_ns::str( bp::self ) );

}
