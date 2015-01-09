#ifndef __COCURC_TIMED_CALLBACK_H_
#define __COCURC_TIMED_CALLBACK_H_

#include <functional>

#include <runnable.h>

#include "timed_idle.h"

namespace cocurc
{
	class timed_callback : public runnable
	{
	public:
		typedef std::function< void() > callback;

	private:
		callback callback_;
		timed_idle< std::chrono::milliseconds > timed_idle_;

	public:
		explicit timed_callback( const size_t delay, const bool first_run, callback cb );
		virtual ~timed_callback();
		//
		virtual void run() final;
	};
}


#endif // __COCURC_TIMED_CALLBACK_H_

