#include "test_registrator.h"

#include <boost/thread.hpp>

#include <timed_idle.h>

namespace cocurc
{
	namespace tests_
	{
		void timed_idle_check_tests()
		{
			bool call = false;
			const auto callback = [ &]() { call = true;  };
			timed_idle< std::chrono::milliseconds > ti( 300, callback, false );

			ti.check();
			BOOST_CHECK_EQUAL( call, false );
			boost::this_thread::sleep( boost::posix_time::milliseconds( 100ul ) );
			ti.check();
			BOOST_CHECK_EQUAL( call, false );
			boost::this_thread::sleep( boost::posix_time::milliseconds( 201ul ) );
			ti.check();
			BOOST_CHECK_EQUAL( call, true );

			call = false;
			boost::this_thread::sleep( boost::posix_time::milliseconds( 300ul ) );
			ti.check();
			BOOST_CHECK_EQUAL( call, true );

		}
	}
}
