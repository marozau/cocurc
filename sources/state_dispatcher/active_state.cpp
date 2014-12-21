#include "active_state.h"

#include "abstract_state.h"

namespace cocurc
{
	active_state::active_state( state_dispatcher& state_dispatcher )
		: state_dispatcher_( state_dispatcher )
	{
	}
	active_state::~active_state()
	{
		runner_.stop();
	}
	//
	void active_state::start( runner::affynity_mask_type mask )
	{
		state_dispatcher_.get_state()->start();
		runner_.start();
		runner_.separate_thread( *this, mask );
	}
	void active_state::stop( const std::string& reason )
	{
		runner_.stop();		
		state_dispatcher_.get_state()->stop();
	}	
	//
	void active_state::run()
	{
		try
		{
			state_dispatcher_.get_state()->run();
		}
		catch ( const std::exception& ex )
		{
			state_dispatcher_.get_state()->error( ex.what() );
		}
		catch ( ... )
		{
			state_dispatcher_.get_state()->error( "unknown expeption" );
		}
	}
}