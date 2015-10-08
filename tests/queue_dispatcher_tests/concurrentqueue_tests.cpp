#include "test_registrator.h"

#include <thread>
#include <atomic>

#include <boost/thread.hpp>

#include <concurrentqueue_dispatcher.h>

namespace cocurc
{
	namespace tests_
	{
		void concurrentqueue_dispatcher_push_tests()
		{
			typedef concurrentqueue_dispatcher< int > test_dispatcher;
			test_dispatcher dispatcher( 1024 );
			int i = 0;
			for ( ; i < 1000; ++i )
				dispatcher.push( std::move( i ) );
			BOOST_CHECK_EQUAL( i > 0, true );
		}

		void concurrentqueue_dispatcher_push_pop_tests()
		{
			typedef concurrentqueue_dispatcher< int > test_dispatcher;
			test_dispatcher dispatcher( 1024 );
			
			std::atomic_bool start;
			start.store( false, std::memory_order::memory_order_release );

			std::atomic_bool stop;
			stop.store( false, std::memory_order::memory_order_release );

			int push_counter = 0;
			std::thread push_thread( [ &]()
			{				
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;
				
				while ( !stop.load( std::memory_order::memory_order_consume ) ) 
				{
					int to_push = ++push_counter;
					dispatcher.push( std::move( to_push ) );
				}
			} );

			struct pop_consumer : public consumer_object < int >
			{
				int pop_counter = 0;
				int diff = 0;
				inline void operator=(int&& obj)
				{
					diff += obj ^ ++pop_counter;
				}
			};

			size_t counter = 0;
			pop_consumer pop;
			std::thread pop_thread( [ &]()
			{
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;				
				while ( !stop.load( std::memory_order::memory_order_consume ) )
					counter += dispatcher.pop_all( pop );
				counter += dispatcher.pop_all( pop );
			} );

			const size_t time_to_sleep = 1000ul;
			start.store( true, std::memory_order::memory_order_release );
			boost::this_thread::sleep( boost::posix_time::milliseconds( time_to_sleep ) );
			stop.store( true, std::memory_order::memory_order_release );

			push_thread.join();
			pop_thread.join();

			BOOST_CHECK_EQUAL( dispatcher.size(), 0 );
			BOOST_CHECK_EQUAL( push_counter, pop.pop_counter );
			BOOST_CHECK_EQUAL( pop.diff, 0 );
			BOOST_TEST_MESSAGE( "***** concurrentqueue perf. tests. ***** \t time: " << time_to_sleep << " msec; count: " << pop.pop_counter );

		}
	}
}
