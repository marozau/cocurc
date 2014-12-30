#ifndef __COCURC_VISITOR_ALIGNED_H_
#define __COCURC_VISITOR_ALIGNED_H_

#include <utility>
#include <type_traits>
#include <memory>

#include <boost/variant.hpp>
#include <boost/none_t.hpp>
#include <boost/variant/polymorphic_get.hpp>

namespace cocurc
{
	template< class Base, class ...Types >
	class visitor_aligned
	{
		typedef typename boost::variant< boost::none_t, Types ... > obj_t;
		obj_t obj_;

	public:
		visitor_aligned() {}		
		template< typename T >
		visitor_aligned( T&& t )
			: obj_( t )
		{
			static_assert(std::is_base_of<Base, T>::value, "T must be derived from Base class");
		}
		// needed for concurentqueue
		visitor_aligned( const visitor_aligned& ) = default;
		visitor_aligned( visitor_aligned&& other )
		{
			*this = std::move( other );
		}
		//
		template< typename T, typename... Args >
		void reset( Args&&... args )
		{
			static_assert( std::is_base_of<Base, T>::value, "T must be derived from Base class" );
			obj_ = std::move( T( std::forward< Args >( args )... ) );
		}

		Base* operator->()
		{
			return &boost::polymorphic_get< Base >( obj_ );
		}
		const Base* operator->() const
		{
			return &boost::polymorphic_get< Base >( obj_ );
		}

	};
}


#endif // __COCURC_VISITOR_ALIGNED_H_
