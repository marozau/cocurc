#include "test_registrator.h"

#include <thread>
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
			
			bool start = false;
			bool stop = false;
			std::thread push_thread( [ & ]()
			{
				while ( !start )
					;
				while ( !stop )
					dispatcher.push( push );
			} );
			std::thread pop_thread( [ & ]()
			{
				while ( !start )
					;
				while ( !stop )
					dispatcher.pop( pop );
				dispatcher.pop( pop );
			} );

			const size_t time_to_sleep = 500ul;
			start = true;
			boost::this_thread::sleep( boost::posix_time::milliseconds( time_to_sleep ) );
			stop = true;
			
			push_thread.join();
			pop_thread.join();

			BOOST_CHECK_EQUAL( push_counter, pop_counter );
			BOOST_MESSAGE( "time: " << time_to_sleep << "; count: " << pop_counter );
		}
	}
}
