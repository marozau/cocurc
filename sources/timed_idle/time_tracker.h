#ifndef _COCURC_TIME_TRACKER_H_
#define _COCURC_TIME_TRACKER_H_

#include <chrono>

namespace cocurc
{
	// time_tracker class count time until it was created or reseted.
	// could be used for performance tests, for processing time calculating
	// not a virtual destructor class
	template< class T >
	class time_tracker
	{
	public:
		typedef long long elapsed_type;

	private:
		elapsed_type start_;
			
	public:
		explicit time_tracker()				
		{
			using namespace std::chrono;
			start_ = duration_cast< T >( high_resolution_clock::now( ).time_since_epoch() ).count();
		}
		time_tracker( const time_tracker& other )			
			: start_( other.start_ )
		{
		}
		~time_tracker()
		{
		}
		//
		void reset()
		{
			using namespace std::chrono;
			start_ = duration_cast< T >( high_resolution_clock::now().time_since_epoch()).count();
		}
		elapsed_type elapsed() const
		{
			using namespace std::chrono;
			const auto now = duration_cast< T >(high_resolution_clock::now( ).time_since_epoch()).count( );
			return now - start_;
		}
	};
}

#endif // _COCURC_TIME_TRACKER_H_
