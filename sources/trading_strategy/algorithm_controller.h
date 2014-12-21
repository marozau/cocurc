#ifndef __COCURC_ALGORITHM_CONTROLLER_H_
#define __COCURC_ALGORITHM_CONTROLLER_H_

#include <abstract_controller.h>

#include "algorithm_visitor.h"
#include "abstract_trading_strategy.h"

namespace cocurc
{
	class algorithm_controller : public abstract_controller< algorithm_visitor< abstract_trading_strategy, tick_data > >
	{
		typedef data_type::algorithm_type algorithm_type;
		algorithm_type& algorithm_;

		queue_dispatcher::pop_functor algo_functor_;
				
	public:
		explicit algorithm_controller( queue_dispatcher& qd, algorithm_type& algorithm );
		virtual ~algorithm_controller();
		//
		virtual void run();
	};
}


#endif // __COCURC_ALGORITHM_CONTROLLER_H_
