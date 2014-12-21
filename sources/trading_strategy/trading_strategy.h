#ifndef _COCURC_TRADING_STRATEGY_STUB_H_
#define _COCURC_TRADING_STRATEGY_STUB_H_

#include "abstract_trading_strategy.h"

namespace cocurc
{
	class trading_strategy : public abstract_trading_strategy
	{
	public:
		explicit trading_strategy();
		virtual ~trading_strategy();
		//
		virtual void on_tick_data_fast( const tick_data& td );
		virtual void on_tick_data_slow( const tick_data& td );
	};	
}

#endif // _COCURC_TRADING_STRATEGY_STUB_H_
