#include "runner.h"

#include <algorithm>

#include "runnable.h"

#ifdef _WIN32 // Windows (x64 and x86)

#include <Windows.h>

namespace cocurc
{
	inline void set_affinity_mask( _In_  HANDLE hThread, _In_  DWORD_PTR affinity_mask )
	{
		SetThreadAffinityMask( hThread, affinity_mask );
	}
}

#elif __linux__ // linux
namespace cocurc
{
	void set_affinity_mask( _In_  HANDLE hThread, _In_  DWORD_PTR affinity_mask )
	{

	}
}
#endif

namespace cocurc
{
	runner::runner()
		: stopping_( false )
	{				
	}

	runner::~runner()
	{
		stop();
	}

	void runner::start()
	{
		stopping_ = false;
	}

	void runner::stop()
	{				
		if ( stopping_ )
			return;

		stopping_ = true;
		std::for_each( thread_storage_.begin(), thread_storage_.end(), []( std::thread& thread )
		{
			if ( thread.joinable() )
				thread.join();
		} );
		thread_storage_.clear();
	}

	// be carefull! runnable::run must be exception safe
	void runner::separate_thread( runnable& to_run, const affynity_mask_type affinity_mask )
	{
		if ( stopping_ )
			throw std::logic_error( "runner was stopped" );

		thread_storage_.push_back( std::thread( [ & ]()	{ this_thread_( to_run ); } ) );

		set_affinity_mask( thread_storage_.back().native_handle(), affinity_mask );
	}

	void runner::separate_thread( runnable& to_run, callback cb, const affynity_mask_type affinity_mask )
	{
		if ( stopping_ )
			throw std::logic_error( "runner was stopped" );

		thread_storage_.push_back( std::thread( [ &, cb ]()	
		{ 
			this_thread_( to_run ); 
			cb();
		} ) );

		set_affinity_mask( thread_storage_.back().native_handle(), affinity_mask );
	}
	
	void runner::this_thread_( runnable& to_run )
	{
		while ( !stopping_ )
		{
			to_run.run();
		}
	}
}
