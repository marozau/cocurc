#ifndef _COCURC_RUNNER_BATCH_H_
#define _COCURC_RUNNER_BATCH_H_

#include <vector>
#include <memory>

#include "runnable.h"

namespace cocurc
{
	class batch : public runnable
	{
	public:
		typedef std::reference_wrapper< runnable > runnable_ref;
		typedef std::vector< runnable_ref > runnable_storage;
		runnable_storage runnable_storage_;

	public:
		void add( runnable& to_run );
		//
		virtual void run();
	};

	class batch_ptr : public runnable
	{
	public:
		typedef std::shared_ptr< runnable > runnable_ptr;
		typedef std::vector< runnable_ptr > runnable_storage;
		runnable_storage runnable_storage_;

	public:
		void add( runnable* to_run );
		//
		virtual void run();
	};
}

#endif // _COCURC_RUNNER_BATCH_H_
