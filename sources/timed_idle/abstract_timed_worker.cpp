#include "abstract_timed_worker.h"

namespace cocurc
{
	abstract_timed_worker::abstract_timed_worker( const size_t delay, const bool first_run )
		: timed_idle_( delay, [ this ](){ job_(); }, first_run )
	{

	}
	abstract_timed_worker::~abstract_timed_worker()
	{
	}
	//
	void abstract_timed_worker::run()
	{
		timed_idle_.check();
	}
}
