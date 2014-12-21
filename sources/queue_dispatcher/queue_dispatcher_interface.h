#ifndef _COCURC_QUEUE_DISPATCHER_INTERFACE_H_
#define _COCURC_QUEUE_DISPATCHER_INTERFACE_H_

#include <functional>

namespace cocurc
{
	template< class data_type >
	class queue_dispatcher_interface
	{
	public:
		typedef data_type data_type;
		typedef typename std::function< void( const data_type& data ) > pop_functor;

	public:
		virtual ~queue_dispatcher_interface() {}
		//
		virtual bool try_push( const data_type&& data ) = 0;
		virtual void push( const data_type&& data ) = 0;
		virtual size_t pop( const pop_functor func ) = 0;
		//
		virtual void stop() = 0;
		//
		virtual size_t size() = 0;
	};
}

#endif // _COCURC_QUEUE_DISPATCHER_INTERFACE_H_
