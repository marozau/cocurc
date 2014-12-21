#ifndef _COCURC_STATE_DISPATCHER_STATE_DISPATCHER_H_
#define _COCURC_STATE_DISPATCHER_STATE_DISPATCHER_H_

#include <memory>
#include <unordered_map>

namespace cocurc
{
	class abstract_state;
	class state_dispatcher
	{
	public:
		typedef std::shared_ptr< abstract_state > state_ptr;
		typedef std::unordered_map< size_t, state_ptr > state_storage;

	private:
		state_storage state_storage_;
		state_ptr state_;

	public:
		void emplace( const size_t key, abstract_state* state );
		void set_state( const size_t key );
		const state_ptr get_state() const;
	};
}

#endif // _COCURC_STATE_DISPATCHER_STATE_DISPATCHER_H_
