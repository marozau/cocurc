#ifndef __COCURC_TIMED_IDLE_H_
#define __COCURC_TIMED_IDLE_H_

#include <functional>

#include "time_tracker.h"

namespace cocurc
{
	template< class T >
	class timed_idle
	{
	public:
		typedef std::function< void() > callback;
		callback cb_;
		callback sleep_strategy_;

		typedef typename time_tracker< T > tt;
		typedef typename tt::elapsed_type idle_time_type;

	private:
		tt tt_;
		idle_time_type idle_time_;
		idle_time_type original_idle_time_;

	public:
		explicit timed_idle( const idle_time_type idle_time, 
							callback cb, 
							const bool first_run )
			: cb_( cb )
			, idle_time_( idle_time )
			, sleep_strategy_( [](){} )
			, original_idle_time_( idle_time )
		{
			if ( first_run )
				idle_time_ = 0;
		}

		void check()
		{
			if ( tt_.elapsed() >= idle_time_ )
			{
				idle_time_ = original_idle_time_;
				tt_.reset();
				cb_();
			}
			else
				sleep_strategy_();
		}

		void set_sleep_strategy(callback sleep_strategy) 
		{
			sleep_strategy_ = sleep_strategy;
		}
	};
}


#endif // __COCURC_TIMED_IDLE_H_

