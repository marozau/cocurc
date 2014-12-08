#ifndef _COCURC_RUNNER_RUNNABLE_H_
#define _COCURC_RUNNER_RUNNABLE_H_

#include <memory>

namespace cocurc
{
	class runnable
	{
	public:
		virtual ~runnable() {}
		virtual void run() = 0;
	};
	typedef std::shared_ptr< runnable > runnable_ptr;
}

#endif // _COCURC_RUNNER_RUNNABLE_H_
