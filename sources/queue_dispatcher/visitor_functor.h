#ifndef __COCURC_VISITOR_FUNCTOR_H_
#define __COCURC_VISITOR_FUNCTOR_H_

#include <functional>

namespace cocurc
{
	template< class algorithm_type, class data_type >
	class visitor_functor
	{		
	public:
		typedef algorithm_type algorithm_type;
		typedef data_type data_type;
		typedef std::function< void( algorithm_type&, const data_type& ) > acceptor_method;
		acceptor_method ac_;		
		data_type data;

	public:
		explicit visitor_functor()
		{
		}

		explicit visitor_functor( acceptor_method ac )
			: ac_( ac )
		{
		}

		void set_acceptor_method( acceptor_method ac )
		{
			ac_ = ac;
		}
		void call( algorithm_type& algorithm ) const
		{
			ac_( algorithm, data );
		}
	};
}


#endif // __COCURC_VISITOR_FUNCTOR_H_
