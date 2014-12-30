#include "test_registrator.h"

#include <thread>
#include <boost/thread.hpp>

#include <visitor_functor.h>
#include <bqueue_dispatcher.h>

namespace cocurc
{
	namespace tests_
	{
		namespace details
		{
			namespace method
			{
				enum value : int
				{
					first,
					second,
					third,

					none
				};
			}
			struct data
			{
				method::value f = method::first;
				method::value s = method::second;
				method::value t = method::third;
			};
			class test_algo
			{				
			public:
				method::value method_called_ = method::none;

				size_t first_counter = 0;
				size_t second_counter = 0;
				size_t third_counter = 0;

			public:
				void first( const data& data )
				{
					method_called_ = data.f;
					++first_counter;
				}

				void second( const data& data )
				{
					method_called_ = data.s;
					++second_counter;
				}

				void third( const data& data )
				{
					method_called_ = data.t;
					++third_counter;
				}
			};
		}

		void visitor_functor_single_method_call_tests()
		{
			typedef visitor_functor< details::test_algo, details::data > visitor;
			details::test_algo algo;
			bqueue_dispatcher< visitor, 8 > dispather;

			const auto push_functor = [ &]( visitor& dest_data )
			{
				dest_data.data.f = details::method::first;
				dest_data.data.s = details::method::second;
				dest_data.data.t = details::method::third;

				visitor::acceptor_method ac = []( visitor::algorithm_type& algo, const visitor::data_type& data ) { algo.first( data ); };
				dest_data.set_acceptor_method( ac );
			};		
			BOOST_CHECK_EQUAL( dispather.try_push( push_functor ), true );

			BOOST_CHECK_EQUAL( algo.method_called_, details::method::none );
			const auto pop_functor = [ &]( const visitor& data )
			{
				data.call( algo );
			};
			BOOST_CHECK_EQUAL( dispather.pop( pop_functor ), 1 );
			BOOST_CHECK_EQUAL( algo.method_called_, details::method::first );
		}

		void visitor_functor_sqenatial_method_call_tests()
		{
			typedef visitor_functor< details::test_algo, details::data > visitor;
			details::test_algo algo;
			bqueue_dispatcher< visitor, 1024 > dispather;

			std::thread push_thread( [ & ]()
			{
				size_t counter = 0;
				const auto push_functor = [ & ]( visitor& dest_data )
				{
					dest_data.data.f = details::method::first;
					dest_data.data.s = details::method::second;
					dest_data.data.t = details::method::third;

					visitor::acceptor_method ac;
					if ( counter % 2 == 0 )
						ac = []( visitor::algorithm_type& algo, const visitor::data_type& data ) { algo.first( data ); };
					else
						ac = []( visitor::algorithm_type& algo, const visitor::data_type& data ) { algo.second( data ); };
					dest_data.set_acceptor_method( ac );
				};
				for ( ; counter < 2048; )
					if ( dispather.try_push( push_functor ) )
						++counter;
			} );

			std::atomic_bool stopping_;
			stopping_.store( false, std::memory_order::memory_order_release );

			BOOST_CHECK_EQUAL( algo.method_called_, details::method::none );

			std::thread pop_thread( [ & ]()
			{				
				const auto pop_functor = [ & ]( const visitor& data )
				{
					data.call( algo );
				};
				
				while ( !stopping_.load( std::memory_order::memory_order_consume ) )
					dispather.pop(pop_functor);
			} );

			push_thread.join();
			stopping_.store( true, std::memory_order::memory_order_release );
			pop_thread.join();

			BOOST_CHECK_EQUAL( algo.first_counter, algo.second_counter );
			BOOST_CHECK_EQUAL( algo.third_counter, 0 );
		}

		void visitor_functor_performance_tests()
		{
			typedef visitor_functor< details::test_algo, details::data > visitor;
			details::test_algo algo;
			bqueue_dispatcher< visitor, 1024 > dispather;

			std::atomic_bool start;
			start.store( false, std::memory_order::memory_order_release );
			std::atomic_bool stopping_;
			stopping_.store( false, std::memory_order::memory_order_release );


			std::thread push_thread( [ &]()
			{
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;

				//uncomment to test memcpy
				/*details::data data;
				data.f = details::method::first;
				data.s = details::method::second;
				data.t = details::method::third;*/

				const auto push_functor = [ &]( visitor& dest_data )
				{
					dest_data.data.f = details::method::first;
					dest_data.data.s = details::method::second;
					dest_data.data.t = details::method::third;

					//uncomment to test memcpy
					//memcpy( &dest_data.data, &data, sizeof( details::data ) );

					visitor::acceptor_method ac = []( visitor::algorithm_type& algo, const visitor::data_type& data ) { algo.first( data ); };
					dest_data.set_acceptor_method( ac );
				};
				
				while ( !stopping_.load( std::memory_order::memory_order_consume ) )
					dispather.try_push( push_functor );
			} );			

			BOOST_CHECK_EQUAL( algo.method_called_, details::method::none );

			std::thread pop_thread( [ &]()
			{
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;

				const auto pop_functor = [ &]( const visitor& data )
				{
					data.call( algo );
				};

				while ( !stopping_.load( std::memory_order::memory_order_consume ) )
					dispather.pop( pop_functor );
				dispather.pop( pop_functor );
			} );

			start.store( true, std::memory_order::memory_order_release );
			const size_t time_to_sleep = 1000ul;
			boost::this_thread::sleep( boost::posix_time::milliseconds( time_to_sleep ) );
			stopping_.store( true, std::memory_order::memory_order_release );

			push_thread.join();			
			pop_thread.join();

			BOOST_TEST_MESSAGE( "****** visitor_functor perf. tests. ***** \t time: " << time_to_sleep << " msec, messages: " << algo.first_counter );
			BOOST_CHECK_EQUAL( algo.second_counter, 0 );
		}
	}
}
