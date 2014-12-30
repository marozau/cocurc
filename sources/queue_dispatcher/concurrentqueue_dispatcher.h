#ifndef __COCURC_CONCURRENTQUEUE_DISPATCHER_H_
#define __COCURC_CONCURRENTQUEUE_DISPATCHER_H_

#include <functional>

#include <concurrentqueue.h>

namespace cocurc
{
	template< class data_type >
	struct consumer_object
	{
		typedef data_type data_type;
		inline void operator=(data_type&& obj);		
	};

	template< class data_type >
	class concurrentqueue_dispatcher
	{
	public:
		typedef data_type data_type;
		typedef typename std::function< void( const data_type& data ) > pop_functor;

	private:
		typedef typename moodycamel::ConcurrentQueue< data_type > queue;
		queue queue_;

		bool stopping_ = false;

	public:
		explicit concurrentqueue_dispatcher( const size_t size )
			: queue_( size )
		{
		}

		virtual ~concurrentqueue_dispatcher() 
		{
		}

		bool try_push( const data_type& data )
		{
			return queue_.try_enqueue( data );
		}

		void push( const data_type& data )
		{
			while ( !try_push( data ) && !stopping_ )
				;
		}
		
		bool try_push( data_type&& data )
		{
			return queue_.try_enqueue( data );
		}

		void push( data_type&& data )
		{
			while ( !try_push( data ) && !stopping_ )
				;
		}

		template< class U >
		bool try_pop( U& consumer )
		{
			return queue_.try_dequeue( consumer );
		}

		template< class U >
		size_t pop_all( U& consumer )
		{
			size_t counter = 0;
			while ( try_pop( consumer ) && !stopping_ )
				++counter;
			return counter;
		}

		void stop()
		{
			stopping_ = true;
		}

		size_t size() const
		{
			return queue_.size_approx();
		}
	};
}
#endif // _COCURC_CONCURRENTQUEUE_DISPATCHER_H_

