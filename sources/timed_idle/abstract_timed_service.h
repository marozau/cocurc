#ifndef __COCURC_SESSION_SERVICE_H_
#define __COCURC_SESSION_SERVICE_H_

#include <runnable.h>

#include "timed_idle.h"

namespace cocurc
{
	class abstract_timed_service : public runnable
	{
		timed_idle< std::chrono::milliseconds > timed_idle_;

	public:
		explicit abstract_timed_service( const size_t delay, const bool first_run );
		virtual ~abstract_timed_service();
		//
		virtual void run();
		//
		virtual void job_() = 0;
	};
}


#endif // #define __COCURC_SESSION_SERVICE_H_

