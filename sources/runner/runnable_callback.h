#ifndef __COCURC_RUNNABLE_CALLBACK_H_
#define __COCURC_RUNNABLE_CALLBACK_H_

#include <functional>

#include "runnable.h"

namespace cocurc
{
	class runnable_callback : public runnable
	{
	public:
		typedef std::function< void() > callback;

	private:
		callback callback_;

	public:
		explicit runnable_callback( callback cb );
		virtual ~runnable_callback();
		//
		virtual void run() final;
	};
}


#endif // __COCURC_RUNNABLE_CALLBACK_H_

