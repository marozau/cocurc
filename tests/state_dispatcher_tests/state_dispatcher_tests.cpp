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
			{
				state_dispatcher < std::string, int > dispatcher;
				BOOST_CHECK_EQUAL( dispatcher.emplace( "one", new int( 1 ) ), true );
				BOOST_CHECK_EQUAL( dispatcher.emplace( "two", new int( 2 ) ), true );
				BOOST_CHECK_EQUAL( dispatcher.emplace( "three", new int( 3 ) ), true );
				BOOST_CHECK_EQUAL( dispatcher.emplace( "four", new int( 4 ) ), true );
				BOOST_CHECK_EQUAL( dispatcher.emplace( "five", new int( 5 ) ), true );
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
				BOOST_CHECK_EQUAL( dispatcher.emplace( one, new int( 1 ) ), true );
				BOOST_CHECK_EQUAL( dispatcher.emplace( two, new int( 2 ) ), true );
				BOOST_CHECK_EQUAL( dispatcher.emplace( three, new int( 3 ) ), true );
				BOOST_CHECK_EQUAL( dispatcher.emplace( four, new int( 4 ) ), true );
				BOOST_CHECK_EQUAL( dispatcher.emplace( five, new int( 5 ) ), true );
			}
		}
		void state_dispatcher_get_state_tests()
		{
			{
				state_dispatcher < std::string, int > dispatcher;
				//const int value = dispatcher.get_state(); //uncomment to get fatal error: memory access violation;
				dispatcher.emplace( "state", new int( 1 ) );
				dispatcher.set_state( "state" );
				BOOST_CHECK_NO_THROW( dispatcher.get_state() );

			}
			{
				state_dispatcher < std::string, int > dispatcher;
				dispatcher.emplace( "one", new int( 1 ) );
				dispatcher.emplace( "two", new int( 2 ) );
				dispatcher.emplace( "three", new int( 3 ) );
				dispatcher.emplace( "four", new int( 4 ) );
				dispatcher.emplace( "five", new int( 5 ) );
			}
		}
	}
}
