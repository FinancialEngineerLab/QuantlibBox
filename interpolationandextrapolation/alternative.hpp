
#pragma once


 

#ifndef Alternative_forward_term_structure_hpp

#define Alternative_forward_term_structure_hpp


 

#include <boost/test/unit_test.hpp>

#include <ql/quantlib.hpp>

#include <ql/indexes/ibor/krwcd.hpp>

#include <ql/time/calendars/southkorea.hpp>

#include <test-suite/utilities.hpp>


 

class AlternativeUltimateForwardTermStructure

{

public:

        static void BaseCurve();

        static void InterpolatedCurve();

        //static void ExtrapolatedForward();

        static void ExtrapolatedLPForward();

        //static void ZeroRateAtFSP();

        //static void testExceptionWhenFSPLessOrEqualZero();

        //static void testObservability();


 

        static boost::unit_test_framework::test_suite* suite();

};


 

#endif

