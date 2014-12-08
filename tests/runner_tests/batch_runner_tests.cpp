#include "test_registrator.h"

#include <boost/thread.hpp>

#include <batch_runner.h>
#include <batch.h>

namespace cocurc
{
	namespace tests_
	{
		namespace details
		{
			class batch_unit : public runnable
			{
				size_t& counter_;

			public:
				explicit batch_unit( size_t& counter )
					: counter_( counter )
				{

				}
				virtual void run()
				{
					++counter_;
				}
			};
		}
		void bach_runner_separate_thread_tests()
		{
			size_t counter = 0;
			batch new_batch;
			runnable_ptr ptr1( new details::batch_unit( counter ) );
			new_batch.add( *ptr1 );
			runnable_ptr ptr2( new details::batch_unit( counter ) );
			new_batch.add( *ptr2 );
			runnable_ptr ptr3( new details::batch_unit( counter ) );
			new_batch.add( *ptr3 );
			runnable_ptr ptr4( new details::batch_unit( counter ) );
			new_batch.add( *ptr4 );
			
			batch_runner runner;
			runner.separate_thread( new_batch );
			
			boost::this_thread::sleep( boost::posix_time::milliseconds( 400ul ) );

			runner.stop();

			BOOST_CHECK_EQUAL( counter % 4 == 0, true );
		}
	}
}
