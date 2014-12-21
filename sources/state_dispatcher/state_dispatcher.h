#ifndef _COCURC_STATE_DISPATCHER_STATE_DISPATCHER_H_
#define _COCURC_STATE_DISPATCHER_STATE_DISPATCHER_H_

#include <memory>
#include <unordered_map>

namespace cocurc
{
	template< class key_type >
	class abstract_state;

	template< class key_type >
	class state_dispatcher 
	{
	public:
		typedef key_type key_type;
		typedef typename abstract_state< key_type > state_type;
		typedef typename std::shared_ptr< state_type > state_ptr;
		typedef state_type& state_ref;
		typedef typename std::unordered_map< key_type, state_ptr > state_storage;

	private:
		state_storage state_storage_;
		state_ptr state_;

	public:
		void emplace( const key_type key, state_type* state )
		{
			const auto result = state_storage_.emplace( std::make_pair( key, state_ptr( state ) ) ).second;
			if ( !result )
				throw std::invalid_argument( "state_dispatcher::emplace error: key duplication" );
		}
		//
		void set_state( const key_type key )
		{
			const auto cit = state_storage_.find( key );
			if ( cit == state_storage_.end() )
				throw std::invalid_argument( "state_dispatcher::set_default: no such state" );

			state_ = cit->second;
		}
		state_ref get_state() const
		{
			return *state_.get();
		}
	};
}

#endif // _COCURC_STATE_DISPATCHER_STATE_DISPATCHER_H_
