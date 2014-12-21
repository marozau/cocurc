#ifndef _COCURC_TRADING_STRATEGY_H_
#define _COCURC_TRADING_STRATEGY_H_

#include "abstract_trading_strategy.h"

namespace cocurc
{
	class trading_strategy_stub : public abstract_trading_strategy
	{
	public:
		explicit trading_strategy_stub();
		virtual ~trading_strategy_stub();
		//
		virtual void on_tick_data_fast( const tick_data& td );
		virtual void on_tick_data_slow( const tick_data& td );
	};
}


#endif // _COCURC_TRADING_STRATEGY_H_
