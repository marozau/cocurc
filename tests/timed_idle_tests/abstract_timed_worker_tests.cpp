#include "test_registrator.h"

#include <boost/thread.hpp>

#include <abstract_timed_worker.h>

namespace cocurc
{
	namespace tests_
	{
		namespace details
		{
			class test_timed_worker : public abstract_timed_worker
			{
			public:
				bool call = false;
			public:
				explicit test_timed_worker( const size_t delay, const bool first_run )
					: abstract_timed_worker( delay, first_run )
				{

				}
				virtual void job()
				{
					call = true;
				}
			};
		}
		void abstract_timed_worker_job_tests()
		{
			details::test_timed_worker test_worker( 300, false );
			test_worker.run();
			BOOST_CHECK_EQUAL( test_worker.call, false );
			boost::this_thread::sleep( boost::posix_time::milliseconds( 100ul ) );
			test_worker.run();
			BOOST_CHECK_EQUAL( test_worker.call, false );
			boost::this_thread::sleep( boost::posix_time::milliseconds( 201ul ) );
			test_worker.run();
			BOOST_CHECK_EQUAL( test_worker.call, true );

			test_worker.call = false;
			boost::this_thread::sleep( boost::posix_time::milliseconds( 300ul ) );
			test_worker.run();
			BOOST_CHECK_EQUAL( test_worker.call, true );
		}
	}
}
