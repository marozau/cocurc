#include "test_registrator.h"

#include <time_source.h>
#include <time_tracker.h>

using namespace cocurc;

namespace cocurc
{
	namespace tests_
	{
		void sieved_time_source_constructor_tests()
		{
			const sieved_time_source< std::chrono::milliseconds, size_t >::sieve_t default_sieve = sieved_time_source< std::chrono::milliseconds, size_t >::DEFAULT_SIEVE;
			BOOST_CHECK_EQUAL( default_sieve, 127 );
		}

		void time_source_get_current_time_tests()
		{
			{
				time_source< std::chrono::milliseconds, size_t >::get_current_time();
				sieved_time_source< std::chrono::milliseconds, size_t >::get_current_time();
				for ( int i = 0; i < sieved_time_source< std::chrono::milliseconds, size_t >::DEFAULT_SIEVE - 1; ++i )
					sieved_time_source< std::chrono::milliseconds, size_t >::get_current_time();

				const auto current_time1 = sieved_time_source< std::chrono::milliseconds, size_t >::get_current_time();
				const auto current_time2 = time_source< std::chrono::milliseconds, size_t >::get_current_time();

				BOOST_CHECK_EQUAL( (current_time2 - current_time1) < 10, true );
			}

			{
				const sieved_time_source< std::chrono::milliseconds, size_t >::sieve_t new_sieve = 1023;
				time_source< std::chrono::milliseconds, size_t >::get_current_time();
				sieved_time_source< std::chrono::milliseconds, size_t >::get_current_time();
				sieved_time_source< std::chrono::milliseconds, size_t >::set_sieve( new_sieve );
				for ( int i = 0; i < new_sieve - 1; ++i )
					sieved_time_source< std::chrono::milliseconds, size_t >::get_current_time();

				const auto current_time1 = sieved_time_source< std::chrono::milliseconds, size_t >::get_current_time();
				const auto current_time2 = time_source< std::chrono::milliseconds, size_t >::get_current_time();

				BOOST_CHECK_EQUAL( (current_time2 - current_time1) < 10, true );
			}
		}

		namespace details
		{
			template< class TimeSource >
			void time_source_performance_tests_helper( const size_t count )
			{
				for ( size_t i = 0; i < count; ++i )
					const TimeSource::result_t time = TimeSource::get_current_time();
			}
		}

		void time_source_performance_tests()
		{
			const size_t test_size = 1000000;
			{
				const time_tracker< std::chrono::nanoseconds, size_t > tt;
				details::time_source_performance_tests_helper< typename time_source< std::chrono::milliseconds, size_t > >( test_size );
				BOOST_TEST_MESSAGE( "time source - " << tt.elapsed() );
			}

			{
				const time_tracker< std::chrono::nanoseconds, size_t > tt;
				details::time_source_performance_tests_helper< typename sieved_time_source< std::chrono::milliseconds, size_t > >( test_size );
				BOOST_TEST_MESSAGE( "sieved time source - " << tt.elapsed() );
			}
		}
	}
}