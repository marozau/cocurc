#include <boost/test/test_tools.hpp>
#include <dynamic_link_helper.h>

namespace cocurc
{
	namespace tests_
	{
		void bqueue_dispatcher_push_tests();
		void bqueue_dispatcher_push_pop_tests();
		//
		void visitor_functor_single_method_call_tests();
		void visitor_functor_sqenatial_method_call_tests();
		void visitor_functor_performance_tests();
		//
		void concurrentqueue_dispatcher_push_tests();
		void concurrentqueue_dispatcher_push_pop_tests();
		//
		void visitor_aligned_constructor_tests();
		void visitor_aligned_performance_tests();
		void visitor_aligned_concurrentqueue_dispatcher_performance_tests();
	}
}
