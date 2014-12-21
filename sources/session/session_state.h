#ifndef _ALFA_MT4_PLUGIN_HD_SESSION_STATE_H_
#define _ALFA_MT4_PLUGIN_HD_SESSION_STATE_H_

#include "abstract_state.h"

namespace cocurc
{
	namespace state
	{
		enum type : int
		{
			connecting,
			idle,
			working
		};
	}

	class algorithm_controller;
	class network_interface;
	class idle : public cocurc::abstract_state< state::type >
	{
	public:
		explicit idle( state_dispatcher& dispatcher, const cocurc::batch_ptr& batch );
		virtual ~idle() {};
		//
		virtual void start();
		virtual void stop();
		virtual void error( const std::string& message );
		//
		virtual const key type() const;
	};
	//
	class connecting : public cocurc::abstract_state< state::type >
	{
		network_interface& ni_;

	public:
		explicit connecting( state_dispatcher& dispatcher, const cocurc::batch_ptr& batch, network_interface& ni );
		virtual ~connecting();
		//
		virtual void start();
		virtual void stop();
		//
		virtual void error( const std::string& message );
		//
		virtual const key type() const;
	};
	//			
	class working : public cocurc::abstract_state< state::type >
	{
		network_interface& ni_;

	public:
		explicit working( state_dispatcher& dispatcher, const cocurc::batch_ptr& batch, network_interface& ni );
		virtual ~working() {};
		//
		virtual void start();
		virtual void stop();
		//
		virtual void error( const std::string& message );
		//
		virtual const key type() const;
	};
}

#endif // _ALFA_MT4_PLUGIN_HD_SESSION_STATE_H_
