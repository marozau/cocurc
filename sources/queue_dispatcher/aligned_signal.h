#ifndef __COCURC_ABSTRACT_SIGNAL_SIGNAL_H_
#define __COCURC_ABSTRACT_SIGNAL_SIGNAL_H_

#include <utility>

#include "boost/variant.hpp"
#include "boost/variant/polymorphic_get.hpp"

namespace cocurc
{
	template< class Base, class ...Types >
	class aligned_signal
	{
		typedef boost::variant< boost::none_t, typename ...Types > obj_t;
		obj_t obj_;

	public:
		aligned_signal() = default;
		aligned_signal( const abstract_signal_creator & ) = default;
		aligned_signal( abstract_signal_creator && ) = default;
		//
		template< typename T, typename... Args >
		void set( Args&&... args )
		{
			obj_( T( std::forward< Args >( args )... ) );
		}

		Base* operator->()
		{
			return &boost::polymorphic_get< Base >( obj_ );
		}
	};
}


#endif // __COCURC_ABSTRACT_SIGNAL_SIGNAL_H_
