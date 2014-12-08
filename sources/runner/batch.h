#ifndef _COCURC_RUNNER_BATCH_H_
#define _COCURC_RUNNER_BATCH_H_

#include <vector>

#include "runnable.h"

namespace cocurc
{
	class batch : public runnable
	{
	public:
		typedef std::vector< std::reference_wrapper< runnable > > runnable_storage;
		runnable_storage runnable_storage_;

	public:
		void add( runnable& to_run );
		//
		virtual void run();
	};
}

#endif // _COCURC_RUNNER_BATCH_H_
