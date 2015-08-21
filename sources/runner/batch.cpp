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
		static auto lambda = []( runnable& to_run )	{ to_run.run();	};
		std::for_each( runnable_storage_.cbegin(), runnable_storage_.cend(), lambda );
	}
	//
	void batch_ptr::add( runnable* const to_run )
	{
		if ( to_run == nullptr )
			throw std::invalid_argument( "batch_ptr::add error: nullptr" );
		runnable_storage_.emplace_back( runnable_ptr( to_run ) );
	}

	void batch_ptr::add( const runnable_ptr to_run )
	{
		if ( !to_run )
			throw std::invalid_argument( "batch_ptr::add error: nullptr" );
		runnable_storage_.emplace_back( to_run );
	}
	//
	void batch_ptr::run()
	{
		static auto lambda = []( const runnable_ptr& to_run ) { to_run->run(); };
		std::for_each( runnable_storage_.cbegin(), runnable_storage_.cend(), lambda );
	}
}
