#ifndef _COCURC_TIME_TRACKER_H_
#define _COCURC_TIME_TRACKER_H_

#include "time_source.h"

namespace cocurc
{
	// time_tracker class count time until it was created or reseted.
	// could be used for performance tests, for processing time calculating
	// not a virtual destructor class
	template< class T, class R, template< typename, typename > class time_source_type = time_source >
	class time_tracker
	{
	public:
		typedef typename time_source_type< T, R > time_source;
		typedef typename time_source::result_t result_t;

	private:
		result_t start_;
			
	public:		
		time_tracker()
		{
			start_ = time_source::get_current_time();
		}

		~time_tracker()
		{
		}
		//
		void reset()
		{
			start_ = time_source::get_current_time();
		}
		result_t elapsed() const
		{
			return time_source::get_time_diff( start_ );
		}
	};
}

#endif // _COCURC_TIME_TRACKER_H_
