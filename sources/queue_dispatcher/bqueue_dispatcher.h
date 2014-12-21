#ifndef __COCURC_BQUEUE_DISPATCHER_H_
#define __COCURC_BQUEUE_DISPATCHER_H_

#include <bqueue.h>

#include "queue_dispatcher_interface.h"

namespace cocurc
{
	template< class data_type, size_t SIZE = 1024 >
	class bqueue_dispatcher : public queue_dispatcher_interface< data_type >
	{
		typedef typename bqueue< data_type, SIZE > queue;
		queue queue_;

		bool stopping_;

	public:
		explicit bqueue_dispatcher() {}
		virtual ~bqueue_dispatcher() {}
		//
		bool try_push( const push_functor func )
		{
			//const auto functor = [ & ]( data_type& destination )
			//{
			//	memcpy( &destination, &data, sizeof( data_type ) );
			//};			
			return queue_.push( func );

		}
		void push( const push_functor func )
		{
			/*const auto functor = [ &]( data_type& destination )
			{
				memcpy( &destination, &data, sizeof( FeedTick ) );
			};*/
			while ( !queue_.push( func ) && !stopping_ )
				;
		}

		size_t pop( const pop_functor func )
		{
			/*int counter = -1;
			const auto functor = [ &]( const FeedTick& ft )
			{
				++counter;
				const size_t i = counter % max_tick_size;
				memcpy( &ticks[ i ], &ft, sizeof( FeedTick ) );
			};
			queue_.consume_all( functor );
			return counter + 1;*/

			return queue_.consume_all( func );
		}

		void stop()
		{
			stopping_ = true;
		}
		//
		size_t size()
		{
			return static_cast< size_t >( queue_.empty() );
		}
	};
}
#endif //_COCURC_BQUEUE_DISPATCHER_H_

