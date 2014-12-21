#ifndef __COCURC_BOOST_LOCK_FREE_QUEUE_DISPATCHER_H_
#define __COCURC_BOOST_LOCK_FREE_QUEUE_DISPATCHER_H_

#include <functional>

#include <boost/lockfree/queue.hpp>

namespace cocurc
{
	template< class data_type >
	class boost_lock_free_queue_dispatcher
	{
	public:
		typedef data_type data_type;
		typedef typename std::function< void( const data_type& data ) > pop_functor;

	private:
		typedef typename boost::lockfree::queue< const data_type > lock_free_queue;
		lock_free_queue queue_;

		bool stopping_;

	public:
		explicit boost_lock_free_queue_dispatcher( const size_t size )
			: queue_( size )
		{
		}
		virtual ~boost_lock_free_queue_dispatcher() {}
		//
		bool try_push( const data_type&& data )
		{
			return queue_.push( data );
		}
		void push( const data_type&& data )
		{
			while ( !queue_.push( data ) && !stopping_ )
				;
		}

		size_t pop( const pop_functor func )
		{
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
#endif //__COCURC_BOOST_LOCK_FREE_QUEUE_DISPATCHER_H_

