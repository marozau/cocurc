#include "batch_runner.h"

#include <algorithm>

#include "batch.h"

namespace cocurc
{
	void batch_runner::start()
	{
		runner_.start();
	}
	void batch_runner::stop()
	{
		runner_.stop();

		batch_storage_.clear();
	}
	//
	void batch_runner::separate_thread( const batch& to_run, const affynity_mask_type affinity_mask )
	{
		batch_storage_.push_back( to_run );
		runner_.separate_thread( batch_storage_.back(), affinity_mask );
	}
	void batch_runner::separate_thread( const batch& to_run, callback cb, const affynity_mask_type affinity_mask )
	{
		batch_storage_.push_back( to_run );
		runner_.separate_thread( batch_storage_.back(), cb, affinity_mask );
	}
}