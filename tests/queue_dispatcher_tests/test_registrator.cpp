#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();

	using namespace cocurc::tests_;
	master_test_suite.add( BOOST_TEST_CASE( &bqueue_dispatcher_push_tests ) );

#ifdef RUN_PERFORMANCE_TESTS
	master_test_suite.add( BOOST_TEST_CASE( &bqueue_dispatcher_push_pop_tests ) );
#endif

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC
