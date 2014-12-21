#ifndef _COCURC_LISTENER_CONTROLLER_H_
#define _COCURC_LISTENER_CONTROLLER_H_

#include <abstract_controller.h>

#include "abstract_trading_strategy.h"

namespace cocurc
{	
	class network_listener_interface
	{
	public:
		virtual ~network_listener_interface();
		//
		virtual bool is_any_data() = 0;
		virtual bool recv() = 0;
	};

	class listener_controller : public abstract_controller< tick_data >
	{				
		network_listener_interface& listener_;

	public:
		explicit listener_controller( queue_dispatcher& qd, network_listener_interface& listener );
		virtual ~listener_controller();
		//
		virtual void run();
	};
}

#endif // _COCURC_LISTENER_CONTROLLER_H_
