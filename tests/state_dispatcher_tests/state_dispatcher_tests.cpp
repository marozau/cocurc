#include "test_registrator.h"

#include <string>

#include <state_dispatcher.h>
#include <abstract_state.h>
#include <batch.h>

using namespace cocurc;

namespace cocurc
{
	namespace tests_
	{
		namespace details
		{
			namespace state
			{
				enum type : size_t
				{
					first,
					second,
					third
				};
			}

			class test_state : public abstract_state
			{
			public:
				explicit test_state( state_dispatcher& dispatcher, const batch_ptr& batch )
					: abstract_state( dispatcher, batch )
				{
				}
				virtual ~test_state() {};
				//
				virtual void start() {}
				virtual void stop() {}
				virtual void error(const std::string& message) {}
			};
			class test_state_1 : public test_state
			{
			public:
				explicit test_state_1( state_dispatcher& dispatcher, const batch_ptr& batch )
					: test_state( dispatcher, batch )
				{
				}
				virtual ~test_state_1() {};
				virtual const size_t type() const
				{
					return state::first;
				}
			};
			class test_state_2 : public test_state
			{
			public:
				explicit test_state_2( state_dispatcher& dispatcher, const batch_ptr& batch )
					: test_state( dispatcher, batch )
				{}
				virtual ~test_state_2() {}
				virtual const size_t type() const
				{
					return state::second;
				}
			};
		}
		void state_dispatcher_emplace_test()
		{
			{
				batch_ptr batch;
				state_dispatcher dispatcher;
				BOOST_CHECK_NO_THROW( dispatcher.emplace( details::state::first, new details::test_state_1( dispatcher, batch ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( details::state::second, new details::test_state_2( dispatcher, batch ) ) );

				BOOST_CHECK_THROW( dispatcher.emplace( details::state::first, new details::test_state_1( dispatcher, batch ) ), std::invalid_argument );
			}
		}
		void state_dispatcher_set_state_tests()
		{
			{
				batch_ptr batch;
				state_dispatcher dispatcher;
				BOOST_CHECK_NO_THROW( dispatcher.emplace( details::state::first, new details::test_state_1( dispatcher, batch ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( details::state::second, new details::test_state_2( dispatcher, batch ) ) );

				BOOST_CHECK_NO_THROW( dispatcher.set_state( details::state::first ) );
				BOOST_CHECK_THROW( dispatcher.set_state( details::state::third ), std::invalid_argument );
			}
		}
		void state_dispatcher_get_state_tests()
		{
			{
				batch_ptr batch;
				state_dispatcher dispatcher;
				BOOST_CHECK_NO_THROW( dispatcher.emplace( details::state::first, new details::test_state_1( dispatcher, batch ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( details::state::second, new details::test_state_2( dispatcher, batch ) ) );

				BOOST_CHECK_EQUAL( dispatcher.get_state().get() == nullptr, true );
				dispatcher.set_state( details::state::first );
				BOOST_CHECK_EQUAL( dispatcher.get_state().get() == nullptr, false );
			}
		}
	}
}
