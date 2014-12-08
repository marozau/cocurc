#include "test_registrator.h"

#include <thread>
#include <boost/thread.hpp>

#include <bqueue.h>

namespace cocurc
{
	namespace tests_
	{
		void bqueue_performance_tests()
		{
			cocurc::bqueue< size_t, 128 > queue;
			bool stopping_ = false;

			size_t producer_counter = 0;
			std::thread producer( [ & ]()
			{
				const auto functor = [ &]( size_t& data )
				{
					data = ++producer_counter;
				};
				while ( !stopping_ )
				{
					while( !queue.push( functor ) );
				}
			} );

			size_t consumer_counter = 0;
			std::thread consumer( [ & ]()
			{
				int i = 0;
				const auto functor = [ &]( const size_t& data )
				{
					BOOST_CHECK_EQUAL( data, ++consumer_counter );
				};
				while ( !stopping_ )
				{
					queue.consume_all( functor );
				}
				queue.consume_all( functor );
			} );

			boost::this_thread::sleep( boost::posix_time::milliseconds( 500ul ) );

			stopping_ = true;

			producer.join();
			consumer.join();

			BOOST_CHECK_EQUAL( producer_counter, 1 );
			BOOST_CHECK_EQUAL( consumer_counter, 1 );
			BOOST_CHECK_EQUAL( consumer_counter, producer_counter );
		}
	}
}
