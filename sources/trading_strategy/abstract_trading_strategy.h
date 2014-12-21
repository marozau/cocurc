#ifndef __COCURC_ABSTRACT_TRADING_STRATEGY_H_
#define __COCURC_ABSTRACT_TRADING_STRATEGY_H_

namespace cocurc
{
	struct tick_data
	{
		float bid_price;
		float ask_price;

		int bid_volume;
		int ask_volume;

		char symbol[ 16 ];
	};

	class abstract_trading_strategy
	{
	public:
		virtual ~abstract_trading_strategy() {}
		//
		virtual void on_tick_data_fast( const tick_data& td ) = 0;
		virtual void on_tick_data_slow( const tick_data& td ) = 0;
	};
}


#endif // __COCURC_ABSTRACT_TRADING_STRATEGY_H_
