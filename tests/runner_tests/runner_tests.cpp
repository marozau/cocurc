#include "test_registrator.h"

#include <memory>

#include <boost/thread.hpp>

#include <runner.h>
#include <runnable.h>

using namespace cocurc;

namespace cocurc
{
	namespace tests_
	{
		namespace details
		{
			class test_run : public runnable
			{
			public:
				bool runned_ = false;

			public:
				virtual ~test_run()
				{
					int i = 9;
				}
				virtual void run()
				{
					runned_ = true;
				}
			};
			typedef std::shared_ptr< test_run > run_ptr;

			class int_counter : public runnable
			{
			public:
				int counter_;

			public:
				explicit int_counter()
					: counter_( 0 )
				{

				}
				virtual ~int_counter()
				{
				}
				virtual void run()
				{
					++counter_;
				}
			};
			class simple_counter : public runnable
			{
			public:
				std::shared_ptr< int_counter > counter_;

			public:
				explicit simple_counter()
				{
					counter_.reset( new int_counter );
				}
				virtual ~simple_counter()
				{
				}
				virtual void run()
				{
					counter_->run();
				}
			};
			typedef std::shared_ptr< simple_counter > simple_counter_ptr;

			class atomic_counter : public runnable
			{
			public:
				std::shared_ptr< int_counter > counter_;

			public:
				explicit atomic_counter()
				{
					counter_.reset( new int_counter );
				}
				virtual ~atomic_counter()
				{
				}
				virtual void run()
				{
					std::atomic_load( &counter_ )->run();
				}
			};
			typedef std::shared_ptr< atomic_counter > atomic_counter_ptr;
			//
			class idle_runner : public runnable
			{					
			public:
				explicit idle_runner()
				{
				}
				virtual ~idle_runner()
				{
				}
				virtual void run()
				{
					//std::this_thread::yield;
					boost::this_thread::sleep( boost::posix_time::milliseconds( 50ul ) );
				}
			};
			typedef std::shared_ptr< idle_runner > idle_runner_ptr;
		}
		
		void runner_separate_thread_tests()
		{
			{
				details::run_ptr run( new details::test_run() );
				runner test_runner;
				BOOST_CHECK_EQUAL( run->runned_, false );
				BOOST_CHECK_NO_THROW( test_runner.separate_thread( *run ) );
				boost::this_thread::sleep( boost::posix_time::milliseconds( 50ul ) );
				BOOST_CHECK_EQUAL( run->runned_, true );
			}
			{
				details::run_ptr run( new details::test_run() );
				runner test_runner;
				BOOST_CHECK_EQUAL( run->runned_, false );

				bool callback_was_called = false;
				runner::callback cb = [ &callback_was_called ]() { callback_was_called = true; };
				BOOST_CHECK_NO_THROW( test_runner.separate_thread( *run, cb ) );
				boost::this_thread::sleep( boost::posix_time::milliseconds( 50ul ) );
				BOOST_CHECK_EQUAL( run->runned_, true );
				test_runner.stop();
				BOOST_CHECK_EQUAL( callback_was_called, true );
			}
		}

		void runner_performance_tests()
		{
			{
				details::simple_counter_ptr run( new details::simple_counter() );
				runner test_runner;
				BOOST_CHECK_NO_THROW( test_runner.separate_thread( *run ) );
				boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );
				test_runner.stop();
				BOOST_TEST_MESSAGE( "counter: " << run->counter_->counter_ );
			}
			{
				details::atomic_counter_ptr run( new details::atomic_counter() );
				runner test_runner;
				BOOST_CHECK_NO_THROW( test_runner.separate_thread( *run ) );
				boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );
				test_runner.stop();
				BOOST_TEST_MESSAGE( "counter: " << run->counter_->counter_ );
			}
			{ // test processor usage
				details::idle_runner_ptr run( new details::idle_runner() );
				runner test_runner;
				BOOST_CHECK_NO_THROW( test_runner.separate_thread( *run ) );
				boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );
			}
		}
	}
}
