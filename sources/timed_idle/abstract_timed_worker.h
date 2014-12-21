#ifndef __COCURC_active_state_WORKER_H_
#define __COCURC_active_state_WORKER_H_

#include <runnable.h>

#include "timed_idle.h"

namespace cocurc
{
	class abstract_timed_worker : public runnable
	{
		timed_idle< std::chrono::milliseconds > timed_idle_;

	public:
		explicit abstract_timed_worker( const size_t delay, const bool first_run );
		virtual ~abstract_timed_worker();
		//
		virtual void run();
		//
		virtual void job() = 0;
	};
}


#endif // #define __COCURC_active_state_WORKER_H_

