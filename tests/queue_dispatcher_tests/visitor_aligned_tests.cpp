#include "test_registrator.h"

#include <thread>
#include <atomic>
#include <boost/thread.hpp>

#include <visitor_aligned.h>
#include <bqueue_dispatcher.h>
#include <concurrentqueue_dispatcher.h>

namespace cocurc
{
	namespace tests_
	{
		namespace details
		{
			class base
			{
			public:
				~base() {}
				virtual int call() = 0;
			};

			namespace dtype
			{
				enum value
				{
					d1,
					d2,
					d3
				};
			}
			class d1 : public base
			{
				int i_ = 0;
			public:
				explicit d1( int i )
					: i_( i )
				{
				}
				~d1() {}
				virtual int call()
				{
					return dtype::d1;
				}
			};
			class d2 : public base
			{
				double i = 23;

			public:
				~d2() {}
				virtual int call()
				{
					return dtype::d2;
				}
			};				
			class visitor_aligned_test_algo
			{
			public:
				size_t first_counter = 0;
				size_t second_counter = 0;

			public:
				void f( const int i )
				{
					++first_counter;
				}

				void s( const double d )
				{
					++second_counter;
				}
			};

			class abstract_signal
			{
			public:
				virtual ~abstract_signal() {}
				//
				virtual void call( visitor_aligned_test_algo& algo ) const = 0;
			};

			class f_call : public abstract_signal
			{
				int i_;
			public:
				explicit f_call( const int i )
					: i_( i )
				{}
				virtual ~f_call() {}
				//
				virtual void call( visitor_aligned_test_algo& algo ) const
				{
					algo.f( i_ );
				}
			};
			class s_call : public abstract_signal
			{
				double d_;
			public:
				explicit s_call( const double d )
					: d_( d )
				{}
				virtual ~s_call() {}
				//
				virtual void call( visitor_aligned_test_algo& algo ) const
				{
					algo.s( d_ );
				}
			};
		}
		void visitor_aligned_constructor_tests()
		{
			typedef visitor_aligned< details::base, details::d1, details::d2 > signal_type;
			{
				signal_type signal;
				signal_type signal1( std::move( signal ) );
			}

			{
				signal_type signal;
				signal.reset< details::d2 >();
				signal_type signal2( std::move( signal ) );
				BOOST_CHECK_EQUAL(signal2->call(), details::dtype::d2);
			}
			{
				signal_type signal;
				signal.reset< details::d1 >( 21 );
				signal_type signal2( std::move( signal ) );
				BOOST_CHECK_EQUAL( signal2->call(), details::dtype::d1 );
				signal2.reset< details::d2 >();
				BOOST_CHECK_EQUAL( signal2->call(), details::dtype::d2 );
			}

			{
				signal_type signal( details::d1( 21 ) );
				signal_type signal2( std::move( signal ) );
				BOOST_CHECK_EQUAL( signal2->call(), details::dtype::d1 );
				signal2 = details::d2();
				BOOST_CHECK_EQUAL( signal2->call(), details::dtype::d2 );
			}
		}

		void visitor_aligned_performance_tests()
		{			
			details::visitor_aligned_test_algo algo;
			typedef visitor_aligned< details::abstract_signal, details::f_call, details::s_call > signal_type;
			bqueue_dispatcher< signal_type, 1024 > dispather;

			std::atomic_bool start;
			start.store( false, std::memory_order::memory_order_release );
			std::atomic_bool stopping_;
			stopping_.store( false, std::memory_order::memory_order_release );

			std::thread push_thread( [ &]()
			{
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;

				int push_counter = 0;
				const auto push_functor = [ & ]( signal_type& signal )
				{					
					signal.reset< details::f_call >( ++push_counter );
					//signal = details::f_call( 21 );
					//counter % 2 ? signal.reset< details::f_call >( ++counter ) : signal.reset< details::s_call >( static_cast< double >( ++counter ) );
				};
				while ( !stopping_.load( std::memory_order::memory_order_consume ) )
					if ( !dispather.try_push( push_functor ) )
						--push_counter;
			} );

			std::thread pop_thread( [ &]()
			{
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;

				const auto pop_functor = [ &]( const signal_type& signal )
				{
					signal->call( algo );
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

			BOOST_CHECK_EQUAL( algo.first_counter > 0, true );
			BOOST_TEST_MESSAGE( "****** visitor_aligned bqueue perf. tests. ***** \t time: " << time_to_sleep << " msec, messages: " << algo.first_counter );
		}

		void visitor_aligned_concurrentqueue_dispatcher_performance_tests()
		{
			details::visitor_aligned_test_algo algo;
			typedef visitor_aligned< details::abstract_signal, details::f_call, details::s_call > signal_type;
			typedef concurrentqueue_dispatcher< signal_type > test_dispatcher;
			test_dispatcher dispatcher( 1024 );

			std::atomic_bool start;
			start.store( false, std::memory_order::memory_order_release );

			std::atomic_bool stop;
			stop.store( false, std::memory_order::memory_order_release );
			
			std::thread push_thread( [ & ]()
			{
				while ( !start.load( std::memory_order::memory_order_consume ) )
					;

				int push_counter = 0;
				while ( !stop.load( std::memory_order::memory_order_consume ) )
					dispatcher.try_push( details::f_call( ++push_counter ) );
			} );

			struct pop_consumer : public consumer_object < signal_type >
			{
				int pop_counter = 0;
				details::visitor_aligned_test_algo& algo_;
				explicit pop_consumer( details::visitor_aligned_test_algo& algo )
					: algo_( algo )
				{}
				inline void operator=(signal_type&& signal )
				{
					signal->call( algo_ );
					++pop_counter;
				}
			};

			size_t counter = 0;
			pop_consumer pop( algo );
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

			BOOST_CHECK_EQUAL( pop.pop_counter > 0, true );
			BOOST_TEST_MESSAGE( "***** visitor_aligned conqueu perf. tests. ***** \t time: " << time_to_sleep << " msec; count: " << pop.pop_counter );

		}
	}
}