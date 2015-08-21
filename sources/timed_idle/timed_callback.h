#ifndef __COCURC_TIMED_CALLBACK_H_
#define __COCURC_TIMED_CALLBACK_H_

#include <functional>

#include <runnable.h>

#include "time_tracker.h"
#include "time_t.h"

namespace cocurc
{
	namespace details
	{
		template< class T, class R, template< typename, typename > class time_source_type = time_source >
		class timed_callback : public runnable
		{
		public:
			typedef std::function< void() > callback;

			typedef typename time_tracker< T, R, time_source_type > time_tracker;
			typedef typename time_tracker::result_t result_t;

		private:
			result_t idle_time_;
			const result_t original_idle_time_;

			callback callback_;
			callback sleep_strategy_;

			time_tracker tt_;

		public:
			explicit timed_callback( const result_t idle_time, const bool first_run = false )
				: idle_time_( idle_time )
				, original_idle_time_( idle_time )
				, callback_( []{} )
				, sleep_strategy_( []{} )
			{
				if ( first_run )
					idle_time_ = 0;
			}

			timed_callback& set_callback( callback cb )
			{
				callback_ = cb;
				return *this;
			}

			timed_callback& set_sleep_strategy( callback strategy )
			{
				sleep_strategy_ = strategy;
				return *this;
			}			

			void reset()
			{
				tt_.reset();
			}

			virtual void run() final
			{
				if ( tt_.elapsed() >= idle_time_ )
				{
					idle_time_ = original_idle_time_;
					tt_.reset();
					callback_();
				}
				else
					sleep_strategy_();
			}
		};
	}

	template< class T > using timed_callback = details::timed_callback < T, cocurc::time_t, time_source >;
	template< class T > using timed_callback_fast = details::timed_callback < T, cocurc::time_t, sieved_time_source >;
}


#endif // __COCURC_TIMED_CALLBACK_H_

