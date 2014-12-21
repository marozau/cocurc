#ifndef __COCURC_ACTIVE_STATE_H_
#define __COCURC_ACTIVE_STATE_H_

#include <runnable.h>
#include <runner.h>

#include "state_dispatcher.h"

namespace cocurc
{
	class queue_dispatcher_interface;
	class active_state : public runnable
	{
		state_dispatcher& state_dispatcher_;
		runner runner_;

	public:
		explicit active_state( state_dispatcher& state_dispatcher );
		virtual ~active_state();
		//
		void start( runner::affynity_mask_type mask = runner::default_affinity_mask );	
		void stop( const std::string& reason );
		//
		virtual void run();
	};
}
#endif // __COCURC_ACTIVE_STATE_H_

