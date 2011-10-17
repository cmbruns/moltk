// This file has been generated by Py++.

#include "boost/python.hpp"
#include "indexing_suite/container_suite.hpp"
#include "indexing_suite/vector.hpp"
#include "wrap_moltk.h"
#include "querypositionlist.pypp.hpp"

namespace bp = boost::python;

void register_QueryPositionList_class(){

    { //::std::vector< moltk::Aligner::QueryPosition* >
        typedef bp::class_< std::vector< moltk::Aligner::QueryPosition* > > QueryPositionList_exposer_t;
        QueryPositionList_exposer_t QueryPositionList_exposer = QueryPositionList_exposer_t( "QueryPositionList" );
        bp::scope QueryPositionList_scope( QueryPositionList_exposer );
        QueryPositionList_exposer.def( bp::indexing::vector_suite< std::vector< moltk::Aligner::QueryPosition* > >::with_policies(bp::return_internal_reference< >()) );
    }

}
