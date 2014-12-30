#include "test_registrator.h"

#include <thread>
#include <atomic>
#include <boost/thread.hpp>

#include "bqueue_dispatcher.h"

namespace cocurc
{
	namespace tests_
	{
		void bqueue_dispatcher_push_tests()
		{
			typedef bqueue_dispatcher< int, 1024 > test_dispatcher;
			test_dispatcher dispatcher;
			int counter = 0;
			const test_dispatcher::push_functor push = [ &counter ]( int& i )
			{
				i = ++counter;
			};
			for ( int i = 0; i < 1000; ++i )
				dispatcher.push( push );

			BOOST_CHECK_EQUAL( counter > 0, true );
		}

		void bqueue_dispatcher_push_pop_tests()
		{
			typedef bqueue_dispatcher< int, 1024 > test_dispatcher;
			test_dispatcher dispatcher;
			int push_counter = 0;
			const test_dispatcher::push_functor push = [ &push_counter ]( int& i )
			{
				i = ++push_counter;
			};

			int pop_counter = 0;
			const test_dispatcher::pop_functor pop = [ &pop_counter ]( const int& i )
			{
				BOOST_CHECK_EQUAL( i, ++pop_counter );
			};
			
			std::atomic_bool start;
			start.store( false, std::memory_order::memory_order_release );

			std::atomic_bool stop;
			stop.store( false, std::memory_order::memory_order_release );

			std::thread push_thread( [ & ]()
			{
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;
				while ( !stop.load( std::memory_order::memory_order_consume ) )
					dispatcher.push( push );
			} );
			std::thread pop_thread( [ & ]()
			{
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;
				while ( !stop.load( std::memory_order::memory_order_consume ) )
					dispatcher.pop( pop );
				dispatcher.pop( pop );
			} );

			const size_t time_to_sleep = 1000ul;
			start.store( true, std::memory_order::memory_order_release );
			boost::this_thread::sleep( boost::posix_time::milliseconds( time_to_sleep ) );
			stop.store( true, std::memory_order::memory_order_release );
			
			push_thread.join();
			pop_thread.join();

			BOOST_CHECK_EQUAL( push_counter, pop_counter );
			BOOST_TEST_MESSAGE( "**** bqueue perf. tests. ***** \t time: " << time_to_sleep << " msec; count: " << pop_counter );
		}
	}
}
