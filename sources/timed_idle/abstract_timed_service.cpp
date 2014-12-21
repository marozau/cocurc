#include "abstract_timed_service.h"

namespace cocurc
{
	abstract_timed_service::abstract_timed_service( const size_t delay, const bool first_run )
		: timed_idle_( delay, [ this ](){ job_(); }, first_run )
	{

	}
	abstract_timed_service::~abstract_timed_service()
	{
	}
	//
	void abstract_timed_service::run()
	{
		timed_idle_.check();
	}
}
