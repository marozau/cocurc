#ifndef _COCURC_BATCH_RUNNER_H_
#define _COCURC_BATCH_RUNNER_H_

#include <vector>

#include "runner.h"

namespace cocurc
{
	class batch;
	class batch_runner
	{
		typedef std::vector< batch > batch_storage;

	public:
		typedef runner::callback callback;

	private:
		batch_storage batch_storage_;
		runner runner_;

	public:
		void start();
		void stop();
		//
		void separate_thread( const batch& to_run, const affynity_mask_type affinity_mask = runner::default_affinity_mask );
		void separate_thread( const batch& to_run, callback cb, const affynity_mask_type affinity_mask = runner::default_affinity_mask );
	};
}

#endif // _COCURC_BATCH_RUNNER_H_
