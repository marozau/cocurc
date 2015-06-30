#include "test_registrator.h"

#include <chrono>

#include <boost/thread.hpp>

#include <timed_callback.h>

namespace cocurc
{
	namespace tests_
	{
		void timed_callback_run_tests()
		{
			bool call = false;
			const auto callback = [ &]() { call = true;  };
			std::unique_ptr< timed_callback< std::chrono::milliseconds> > ti( &((new timed_callback< std::chrono::milliseconds>( 300, false ))->set_callback( callback )) );
			
			ti->run();
			BOOST_CHECK_EQUAL( call, false );
			boost::this_thread::sleep( boost::posix_time::milliseconds( 100ul ) );
			ti->run();
			BOOST_CHECK_EQUAL( call, false );
			boost::this_thread::sleep( boost::posix_time::milliseconds( 201ul ) );
			ti->run();
			BOOST_CHECK_EQUAL( call, true );

			call = false;
			boost::this_thread::sleep( boost::posix_time::milliseconds( 300ul ) );
			ti->run();
			BOOST_CHECK_EQUAL( call, true );

		}
	}
}
