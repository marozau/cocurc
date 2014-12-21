#include "listener_controller.h"

namespace cocurc
{
	listener_controller::listener_controller( queue_dispatcher& qd, network_listener_interface& listener )
		: super( qd )
		, listener_( listener )
	{
	}
	listener_controller ::~listener_controller()
	{
	}
	void listener_controller::run()
	{			
		while ( listener_.is_any_data() )
		{
			listener_.recv(); //should thorw exception in case of dissconetion, bad data ect
		}
	}
}
