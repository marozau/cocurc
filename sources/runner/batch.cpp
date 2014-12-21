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
	//
	void batch_ptr::add( runnable* to_run )
	{
		if ( to_run == nullptr )
			throw std::invalid_argument( "batch_ptr::add error: nullptr" );
		runnable_storage_.push_back( runnable_ptr( to_run ) );
	}
	//
	void batch_ptr::run()
	{
		std::for_each( runnable_storage_.cbegin(), runnable_storage_.cend(), []( runnable_ptr to_run )
		{
			to_run->run();
		} );
	}
}
