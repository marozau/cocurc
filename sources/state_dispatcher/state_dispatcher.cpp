#include "state_dispatcher.h"

#include <string>

#include "abstract_state.h"

namespace cocurc
{
	void state_dispatcher::emplace( const size_t key, abstract_state* state )
	{
		state_ptr new_state( state );
		const auto result = state_storage_.emplace( std::make_pair( key, new_state ) ).second;
		if ( !result )
			throw std::invalid_argument( "state_dispatcher::emplace error: key duplication" );
	}
	//
	void state_dispatcher::set_state( const size_t key )
	{
		const auto cit = state_storage_.find( key );
		if ( cit == state_storage_.end() )
			throw std::invalid_argument( "state_dispatcher::set_default: state with key: " + std::to_string( key ) );

		state_ = cit->second;
	}
	const state_dispatcher::state_ptr state_dispatcher::get_state() const
	{
		return state_;
	}
}