#include "batch.h"

#include <algorithm>

namespace cocurc
{
	void batch::add( runnable& to_run )
	{
		runnable_storage_.push_back( to_run );
	}
	//
	void batch::run()
	{
		std::for_each( runnable_storage_.cbegin(), runnable_storage_.cend(), []( runnable& to_run )
		{
			to_run.run();
		} );
	}
}
