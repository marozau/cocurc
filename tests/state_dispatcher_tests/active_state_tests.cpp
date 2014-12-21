#include "test_registrator.h"

#include <active_state.h>
#include <state_dispatcher.h>
#include <abstract_state.h>

namespace cocurc
{
	namespace tests_
	{
		namespace details
		{
			namespace state
			{
				enum value : size_t
				{
					start,
					stop
				};
			}
			class test_state_start : public abstract_state
			{
			public:
				explicit test_state_start( state_dispatcher& dispatcher, const batch_ptr& batch )
					: abstract_state( dispatcher, batch )
				{
				}
				virtual ~test_state_start() {};
				//
				virtual void start() 
				{
					dispatcher_.set_state( state::stop );
				}
				virtual void stop() 
				{
					throw std::invalid_argument( "start state" );
				}
				virtual void error( const std::string& message ) {}
				//
				virtual const size_t type() const
				{
					return state::start;
				}
			};
			class test_state_stop : public abstract_state
			{
			public:
				explicit test_state_stop( state_dispatcher& dispatcher, const batch_ptr& batch )
					: abstract_state( dispatcher, batch )
				{
				}
				virtual ~test_state_stop() {};
				//
				virtual void start() 
				{
					throw std::logic_error( "stop state" );
				}
				virtual void stop() 
				{
					dispatcher_.set_state( state::start );
				}
				virtual void error( const std::string& message ) {}
				//
				virtual const size_t type() const
				{
					return state::stop;
				}
			};
		}
		void active_state_constructor_tests()
		{
			state_dispatcher sd;
			sd.emplace( details::state::start, new details::test_state_start( sd, batch_ptr() ) );
			sd.emplace( details::state::stop, new details::test_state_stop( sd, batch_ptr() ) );
			sd.set_state( details::state::start );

			std::shared_ptr< active_state > test_active_state;
			BOOST_CHECK_NO_THROW( test_active_state.reset( new active_state( sd ) ) );
		}
		void active_state_start_tests()
		{
			state_dispatcher sd;
			sd.emplace( details::state::start, new details::test_state_start( sd, batch_ptr() ) );
			sd.emplace( details::state::stop, new details::test_state_stop( sd, batch_ptr() ) );
			sd.set_state( details::state::start );

			active_state test_active_state( sd );
			BOOST_CHECK_NO_THROW( test_active_state.start() );
			BOOST_CHECK_THROW( test_active_state.start(), std::logic_error );
		}
		void active_state_stop_tests()
		{
			state_dispatcher sd;
			sd.emplace( details::state::start, new details::test_state_start( sd, batch_ptr() ) );
			sd.emplace( details::state::stop, new details::test_state_stop( sd, batch_ptr() ) );
			sd.set_state( details::state::start );

			active_state test_active_state( sd );
			BOOST_CHECK_THROW( test_active_state.stop( "test"), std::logic_error );
			BOOST_CHECK_NO_THROW( test_active_state.start() );
			BOOST_CHECK_NO_THROW( test_active_state.stop( "test" ) );
		}
	}
}
