#ifndef __COCURC_ABSTRACT_CONTROLLER_H_
#define __COCURC_ABSTRACT_CONTROLLER_H_

#include <runnable.h>

#include <queue_dispatcher_interface.h>

namespace cocurc
{
	template< class data_type >
	class queue_dispatcher_interface;

	template< class data_type >
	class abstract_controller : public runnable
	{
	public:
		typedef typename data_type data_type;
		typedef typename abstract_controller< data_type > super;
		typedef typename queue_dispatcher_interface< data_type > queue_dispatcher;

	protected:
		queue_dispatcher& qd_;
				
	public:
		explicit abstract_controller( queue_dispatcher& qd )
			: qd_( qd )
		{
		}
		virtual ~abstract_controller() {}
	};
}


#endif // __COCURC_ABSTRACT_CONTROLLER_H_
