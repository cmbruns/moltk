// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "Printable.pypp.hpp"

namespace bp = boost::python;

struct Printable_wrapper : moltk::Printable, bp::wrapper< moltk::Printable > {

    Printable_wrapper()
    : moltk::Printable()
      , bp::wrapper< moltk::Printable >(){
        // null constructor
        
    }

    virtual void print_to_stream( ::std::ostream & os ) const {
        bp::override func_print_to_stream = this->get_override( "print_to_stream" );
        func_print_to_stream( boost::ref(os) );
    }

};

void register_Printable_class(){

    bp::class_< Printable_wrapper, boost::noncopyable >( "Printable", "\n moltk::Printable is an abstract base class to help streamline\n string conversion in the moltk python bindings.\n" )    
        .def( 
            "print_to_stream"
            , bp::pure_virtual( (void ( ::moltk::Printable::* )( ::std::ostream & ) const)(&::moltk::Printable::print_to_stream) )
            , ( bp::arg("os") )
            , "\n moltk::Printable is an abstract base class to help streamline\n string conversion in the moltk python bindings.\n" )    
        .def( bp::self_ns::str( bp::self ) );

}
