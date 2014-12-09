#include "state.h"

#include <boost/thread.hpp>

#include "network_interface.h"
#include "logic.h"

namespace cocurc
{
	idle::idle( state_dispatcher& dispatcher, network_interface& ni )
		: super( dispatcher )
		, ni_( ni )
	{
	}
	const idle::super::key idle::type() const
	{
		return state::idle;
	}
	void idle::start()
	{
		dispatcher_.set_state( state::connecting );
	}
	void idle::stop()
	{
	}
	void idle::error( const std::string& message )
	{
	}
	void idle::run()
	{
	}
	//
	//
	connecting::connecting( state_dispatcher& dispatcher, network_interface& ni )
		: super( dispatcher )
		, ni_( ni )
	{
	}
	connecting::~connecting()
	{
	}
	void connecting::start()
	{
		stopping_ = false;;
	}
	void connecting::stop()
	{
		dispatcher_.set_state( state::idle );
		stopping_ = true;
	}
	void connecting::error( const std::string& message )
	{
	}
	const connecting::super::key connecting::type() const
	{
		return state::connecting;
	}
	void connecting::run()
	{
		if ( ni_.connect() )
		{
			if ( ni_.logon() )
			{
				if ( ni_.idle() )
				{
					dispatcher_.set_state( state::working );
					return;
				}
			}
		}
		wait_();
	}
	void connecting::wait_() const
	{
		static const size_t to_sleep = 50ul;
		static const size_t delay = 5000;
		size_t sleep_time = 0;
		while ( !stopping_ && sleep_time < delay )
		{
			boost::this_thread::sleep( boost::posix_time::milliseconds( to_sleep ) );
			sleep_time += to_sleep;
		}
	}
	//
	working::working( state_dispatcher& dispatcher,
					  const batch& to_run )
		: super( dispatcher )
		, to_run_( to_run )
	{
	}
	void working::start()
	{
	}
	void working::stop()
	{
		dispatcher_.set_state( state::idle );
	}
	void working::error( const std::string& message )
	{
		dispatcher_.set_state( state::connecting );
	}
	const working::super::key working::type() const
	{
		return state::working;
	}
	void working::run()
	{
		to_run_.run();
	}
}
