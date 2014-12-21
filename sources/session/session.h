#ifndef __COCURC_SESSION_H_
#define __COCURC_SESSION_H_

#include <state_dispatcher.h>
#include <runnable.h>
#include <runner.h>

#include "abstract_state.h"
#include "session_state.h"

namespace cocurc
{
	namespace tests_
	{
		class session_tests;
	}
	template< class data_type >
	class queue_dispatcher_interface;

	class session : public runnable
	{
		friend class cocurc::tests_::session_tests;

	public:		
		typedef state_dispatcher< state::type > state_dispatcher;
		typedef state_dispatcher::state_type state_type;

	private:
		state_dispatcher& state_dispatcher_;
		runner runner_;

	public:
		explicit session( state_dispatcher& state_dispatcher );
		virtual ~session();
		//
		void start( );	
		void stop( const std::string& reason );
		//
		virtual void run();
	};
}
#endif // _COCURC_SESSION_H_

