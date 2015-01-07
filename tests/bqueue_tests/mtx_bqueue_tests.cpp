#include "test_registrator.h"

#include <thread>
#include <algorithm>
#include <vector>
#include <atomic>
#include <boost/thread.hpp>

#include <mtx_bqueue.h>

namespace cocurc
{
	namespace tests_
	{
		void mtx_bqueue_performance_tests_helper( const size_t threads )
		{
			cocurc::mtx_bqueue< size_t, 128 > queue;
			std::atomic_bool start;
			start.store( false, std::memory_order::memory_order_release );

			std::atomic_bool stopping_;
			stopping_.store( false, std::memory_order::memory_order_release );

			std::vector< std::thread > thread_pool;
			for ( size_t i = 0; i < threads; ++i )
			{
				thread_pool.push_back(
					std::thread( [ &]()
				{
					size_t producer_counter = 0;
					while ( !start.load( std::memory_order::memory_order_consume ) )
						;
					const auto functor = [ &]( size_t& data )
					{
						data = ++producer_counter;
					};
					while ( !stopping_.load( std::memory_order::memory_order_consume ) )
					{
						queue.push( functor );
					}
				} ) );
			}

			size_t consumer_counter = 0;
			std::thread consumer( [ & ]()
			{
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;
				int i = 0;
				const auto functor = [ &]( const size_t& data )
				{
					BOOST_CHECK_EQUAL( data > 0, true );
					++consumer_counter;
				};
				while ( !stopping_.load( std::memory_order::memory_order_consume ) )
				{
					queue.consume_all( functor );
				}
				queue.consume_all( functor );
			} );

			start.store( true, std::memory_order::memory_order_release );
			boost::this_thread::sleep( boost::posix_time::milliseconds( 1000ul ) );
			stopping_.store( true, std::memory_order::memory_order_release );

			std::for_each( thread_pool.begin(), thread_pool.end(), [] ( std::thread& thread)
			{
				thread.join();
			} );
			consumer.join();

			//BOOST_TEST_MESSAGE( "producer: " << producer_counter );
			BOOST_TEST_MESSAGE( "mtx_bqueue, consumer: " << consumer_counter << "; threads: " << threads );
			//BOOST_CHECK_EQUAL( consumer_counter, producer_counter );
		}
		void mtx_bqueue_performance_tests()
		{
			mtx_bqueue_performance_tests_helper( 1 );
			mtx_bqueue_performance_tests_helper( 4 );
		}
	}
}
