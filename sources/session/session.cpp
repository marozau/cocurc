#include "session.h"

namespace cocurc
{
	session::session( state_dispatcher& state_dispatcher )
		: state_dispatcher_( state_dispatcher )
	{
	}
	session::~session()
	{
		runner_.stop();
	}
	//
	void session::start()
	{
		state_dispatcher_.get_state().start();
		runner_.start();
		runner_.separate_thread( *this );
	}
	void session::stop( const std::string& reason )
	{
		runner_.stop();		
		state_dispatcher_.get_state().stop();
	}	
	//
	void session::run()
	{
		try
		{
			state_dispatcher_.get_state().run();
		}
		catch ( const std::exception& ex )
		{
			// todo: tests and think what to do in case of error			
			state_dispatcher_.get_state().error( ex.what() );
		}
		catch ( ... )
		{
			// todo: tests and think what to do in case of error			
			state_dispatcher_.get_state().error( "unknown expeption" );
		}
	}
}