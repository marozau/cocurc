#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );

	using namespace cocurc::tests_;
	master_test_suite.add( BOOST_TEST_CASE( &bqueue_dispatcher_push_tests ) );
	//
	master_test_suite.add( BOOST_TEST_CASE( &visitor_functor_single_method_call_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &visitor_functor_sqenatial_method_call_tests ) );
	//
	master_test_suite.add( BOOST_TEST_CASE( &concurrentqueue_dispatcher_push_tests ) );
	//
	master_test_suite.add( BOOST_TEST_CASE( &visitor_aligned_constructor_tests ) );

#ifdef RUN_PERFORMANCE_TESTS
	master_test_suite.add( BOOST_TEST_CASE( &bqueue_dispatcher_push_pop_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &visitor_functor_performance_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &visitor_aligned_performance_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &visitor_aligned_concurrentqueue_dispatcher_performance_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &concurrentqueue_dispatcher_push_pop_tests ) );
#endif

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC
