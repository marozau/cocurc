#include "session_state.h"

#include "state_dispatcher.h"
#include "network_interface.h"

namespace cocurc
{
	idle::idle( state_dispatcher& dispatcher, const cocurc::batch_ptr& batch )
		: super( dispatcher, batch )
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
	//
	//
	connecting::connecting( state_dispatcher& dispatcher, const batch_ptr& batch, network_interface& ni )
		: super( dispatcher, batch )
		, ni_( ni )
	{
	}
	connecting::~connecting()
	{
	}
	void connecting::start()
	{
	}
	void connecting::stop()
	{
		ni_.logout();
		ni_.disconnect();
		dispatcher_.set_state( state::idle );
	}
	void connecting::error( const std::string& message )
	{
		//logger_->error( "'connecting' state error message: " ) << message;
	}
	const connecting::super::key connecting::type() const
	{
		return state::connecting;
	}
	//void connecting::connect_()
	//{
	//	ni_.disconnect();
	//	if ( ni_.connect() )
	//	{
	//		if ( ni_.logon() )
	//		{
	//			logger_->info( "connected to the proxy" );
	//			dispatcher_.set_state( state::working );
	//			return;
	//		}
	//		throw std::logic_error( "connection failed: wrong login, password or session name" );
	//	}
	//	throw std::logic_error( "connection failed: cannot connect to the proxy" ); // this goes to the connecting::error message
	//}
	//
	working::working( state_dispatcher& dispatcher, const batch_ptr& batch, network_interface& ni )
		: super( dispatcher, batch )
		, ni_( ni )
	{
	}
	void working::start()
	{
	}
	void working::stop()
	{
		ni_.logout();
		ni_.disconnect();
		dispatcher_.set_state( state::idle );
	}
	void working::error( const std::string& message )
	{
		//logger_->error( "'working' state error message: " ) << message;
		dispatcher_.set_state( state::connecting );
	}
	const working::super::key working::type() const
	{
		return state::working;
	}
}