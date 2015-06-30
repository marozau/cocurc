#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );

	using namespace cocurc::tests_;
	master_test_suite.add( BOOST_TEST_CASE( &time_source_get_current_time_tests ) );

	master_test_suite.add( BOOST_TEST_CASE( &timed_callback_run_tests ) );
	//
	master_test_suite.add( BOOST_TEST_CASE( &sieved_time_source_constructor_tests ) );	

#ifdef RUN_PERFORMANCE_TESTS
	master_test_suite.add( BOOST_TEST_CASE( &time_source_performance_tests ) );
#endif

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC