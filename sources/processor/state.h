#ifndef _COCURC_PROCESSOR_STATE_H_
#define _COCURC_PROCESSOR_STATE_H_

#include "abstract_state.h"

namespace cocurc
{
	namespace state
	{
		enum type
		{
			connecting,
			idle,
			working
		};
	}
	//
	class network_interface;
	class network_listener;
	class network_speaker;
	//
	class logic;
	//
	class idle : public abstract_state< state::type >
	{
		network_interface& ni_;

	public:
		explicit idle( state_dispatcher& dispatcher, network_interface& ni );
		virtual ~idle() {};
		//
		virtual void start();
		virtual void stop();
		virtual void error( const std::string& message );
		//
		virtual const key type() const;
		//
		virtual void run();
	};
	//
	class connecting : public abstract_state< state::type >
	{
		network_interface& ni_;

		bool stopping_ = false;

	public:
		explicit connecting( state_dispatcher& dispatcher, network_interface& ni );
		virtual ~connecting();
		//
		virtual void start();
		virtual void stop();
		//
		virtual void error( const std::string& message );
		//
		virtual const key type() const;
		//
		virtual void run();
		
	private:
		void wait_() const;
	};
	//
	class working : public abstract_state< state::type >
	{
		batch to_run_;

	public:
		explicit working( state_dispatcher& dispatcher, 
						  const batch& to_run );
		virtual ~working() {};
		//
		virtual void start();
		virtual void stop();
		//
		virtual void error( const std::string& message );
		//
		virtual const key type() const;
		//
		virtual void run();
	};
	
}

#endif // _COCURC_PROCESSOR_STATE_H_
