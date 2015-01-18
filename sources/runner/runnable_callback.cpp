#include "runnable_callback.h"

namespace cocurc
{
	runnable_callback::runnable_callback( callback cb )
		: callback_( cb )
	{
	}
	runnable_callback ::~runnable_callback()
	{
	}
	//
	void runnable_callback::run()
	{
		callback_();
	}
}
