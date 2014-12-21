#ifndef __COCURC_ALGORITHM_VISITOR_H_
#define __COCURC_ALGORITHM_VISITOR_H_

#include <functional>

namespace cocurc
{
	template< class algorithm_type, class data_type >
	class algorithm_visitor
	{		
	public:
		typedef algorithm_type algorithm_type;
		typedef data_type data_type;
		typedef std::function< void( algorithm_type&, const data_type& ) > acceptor_method;
		acceptor_method ac_;		
		data_type data;

	public:
		explicit algorithm_visitor( acceptor_method ac )
			: ac_( ac )
		{
		}
		void call( algorithm_type& algorithm ) const
		{
			ac_( algorithm, data );
		}
	};
}


#endif // __COCURC_ALGORITHM_VISITOR_H_
