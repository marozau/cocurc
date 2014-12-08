#ifndef _COCURC_PROCESSOR_PROCESSOR_H_
#define _COCURC_PROCESSOR_PROCESSOR_H_

#include <state_dispatcher.h>
#include <runnable.h>
#include <runner.h>

#include "abstract_state.h"

namespace cocurc
{
	template< class key >
	class processor : public runnable
	{
	public:
		typedef typename abstract_state< key >::state_dispatcher state_dispatcher;

	private:
		state_dispatcher& dispatcher_;
		runner runner_;

	public:
		explicit processor( state_dispatcher& dispatcher )
			: dispatcher_( dispather )
		{

		}
		virtual ~processor()
		{
			stop();
		}
		//
		void start()
		{
			dispatcher_.get_state().start();
			runner_.start();
			runner_.separate_thread( *this );
		}
		void stop()
		{
			runner_.stop();
			dispatcher_.get_state().stop();
		}
		//
		virtual void run()
		{
			try
			{
				dispatcher_.get_state().run();
			}
			catch ( const std::exception& ex )
			{
				dispatcher_.get_state().error( ex.what() );
			}
			catch ( ... )
			{
				dispatcher_.get_state().error( "unknown expeption" );
			}
		}
	};
}

#endif // _COCURC_PROCESSOR_PROCESSOR_H_
