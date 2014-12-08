#ifndef _COCURC_PROCESSOR_ABSTRACT_STATE_H_
#define _COCURC_PROCESSOR_ABSTRACT_STATE_H_

#include <string>

#include <state_dispatcher.h>
#include <runnable.h>
#include <batch.h>

namespace cocurc
{
	namespace state_enum
	{
		enum type
		{
			connecting,
			idle,
			working
		};
	}
	template< class key >
	class abstract_state : public runnable
	{
	public:
		typedef key key;
		typedef typename abstract_state< key > super;
		typedef state_dispatcher< key, super > state_dispatcher;

	protected:
		state_dispatcher& dispatcher_;

	public:
		explicit abstract_state( state_dispatcher& dispatcher )
			: dispatcher_( dispatcher )
		{
		}
		virtual ~abstract_state() {}
		//
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void error( const std::string& message ) = 0;
		//
		virtual const key type() const = 0;
	};	
}

#endif // _COCURC_PROCESSOR_ABSTRACT_STATE_H_
