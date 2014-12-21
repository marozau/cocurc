#include "algorithm_controller.h"

namespace cocurc
{
	algorithm_controller::algorithm_controller( queue_dispatcher& qd, algorithm_type& algorithm )
		: super( qd )
		, algorithm_( algorithm )
	{
		algo_functor_ = [ this ]( const data_type& td )
		{
			td.call( algorithm_ );
		};
	}
	algorithm_controller::~algorithm_controller( )
	{
	}
	//
	void algorithm_controller::run( )
	{
		qd_.pop( algo_functor_ );
	}
}
