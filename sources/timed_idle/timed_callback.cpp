#include "timed_callback.h"

namespace cocurc
{
	timed_callback::timed_callback( const size_t delay, const bool first_run, callback cb )
		: timed_idle_( delay, cb, first_run )
	{
	}
	timed_callback::~timed_callback()
	{
	}
	//
	void timed_callback::run()
	{
		timed_idle_.check();
	}
}
