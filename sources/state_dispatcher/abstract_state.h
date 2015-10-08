#ifndef _COCURC_PROCESSOR_ABSTRACT_STATE_H_
#define _COCURC_PROCESSOR_ABSTRACT_STATE_H_

#include <string>

#include <runnable.h>
#include <batch.h>

namespace cocurc
{
	class state_dispatcher;

	class abstract_state : public runnable
	{
	protected:
		state_dispatcher& dispatcher_;
		cocurc::batch_ptr batch_;

	public:
		explicit abstract_state( state_dispatcher& dispatcher, const batch_ptr& batch )
			: dispatcher_( dispatcher )
			, batch_( batch )
		{
		}
		virtual ~abstract_state() {}
		//
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void error( const std::string& message ) = 0;

		virtual bool is_alive() const = 0;
		//
		virtual const size_t type() const = 0;
		//
		virtual void run()
		{
			batch_.run();
		}
	};	
}

#endif // _COCURC_PROCESSOR_ABSTRACT_STATE_H_
