#include "test_registrator.h"

#include <string>

#include <state_dispatcher.h>

using namespace cocurc;

namespace cocurc
{
	namespace tests_
	{
		void state_dispatcher_emplace_test()
		{
			/*{
				state_dispatcher < std::string, int > dispatcher;
				BOOST_CHECK_NO_THROW( dispatcher.emplace( "one", new int( 1 ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( "two", new int( 2 ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( "three", new int( 3 ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( "four", new int( 4 ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( "five", new int( 5 ) ) );

				BOOST_CHECK_THROW( dispatcher.emplace( "five", new int( 5 ) ), std::invalid_argument );
			}
			{
				enum value
				{
					one,
					two,
					three,
					four,
					five
				};
				state_dispatcher < value, int > dispatcher;
				BOOST_CHECK_NO_THROW( dispatcher.emplace( one, new int( 1 ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( two, new int( 2 ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( three, new int( 3 ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( four, new int( 4 ) ) );
				BOOST_CHECK_NO_THROW( dispatcher.emplace( five, new int( 5 ) ) );

				BOOST_CHECK_THROW( dispatcher.emplace( five, new int( 5 ) ), std::invalid_argument );
			}*/
		}
		void state_dispatcher_get_state_tests()
		{
			//{
			//	state_dispatcher < std::string, int > dispatcher;
			//	//const int value = dispatcher.get_state(); //uncomment to get fatal error: memory access violation;
			//	dispatcher.emplace( "state", new int( 1 ) );
			//	dispatcher.set_state( "state" );
			//	BOOST_CHECK_NO_THROW( dispatcher.get_state() );

			//}
			//{
			//	state_dispatcher < std::string, int > dispatcher;
			//	dispatcher.emplace( "one", new int( 1 ) );
			//	dispatcher.emplace( "two", new int( 2 ) );
			//	dispatcher.emplace( "three", new int( 3 ) );
			//	dispatcher.emplace( "four", new int( 4 ) );
			//	dispatcher.emplace( "five", new int( 5 ) );
			//}
		}
	}
}
