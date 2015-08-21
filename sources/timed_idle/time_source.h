#ifndef _COCURC_TIME_SOURCE_H_
#define _COCURC_TIME_SOURCE_H_

#include <chrono>
#include <atomic>
#include <cstdint>

namespace cocurc
{
	template< class Period, class R >
	class time_source
	{
		time_source() = delete;
		time_source( const time_source& ) = delete;

	public:		
		typedef typename Period period;
		typedef typename period::rep rep;
		typedef typename R result_t;

	private:
		static const rep start;

	public:
		static result_t get_current_time()
		{					
			return static_cast< result_t >(std::chrono::duration_cast< period >(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - start);
		}

		static result_t get_time_diff( const result_t& time )
		{
			const result_t current_time = get_current_time();
			return current_time - time;
		}
	};

	template< class Period, class R >
	const typename time_source< Period, R >::rep time_source< Period, R >::start = std::chrono::duration_cast< Period >(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	// usefull in busy spin cycles
	template< class Period = std::chrono::milliseconds, class R = size_t >
	class sieved_time_source
	{
		sieved_time_source() = delete;
		sieved_time_source( const sieved_time_source& ) = delete;

	public:
		typedef typename time_source< Period, R >::period period;
		typedef typename time_source< Period, R >::rep rep;
		typedef typename time_source< Period, R >::result_t result_t;		
		typedef std::int64_t index_t;
		typedef std::atomic< index_t > atomic_index_t;
		typedef std::int32_t sieve_t;
		static sieve_t DEFAULT_SIEVE;

	private:		
		static sieve_t sieve;
		static atomic_index_t index_;

	public:
		static void set_sieve( const sieve_t value )
		{
			sieve = value;
		}

		static result_t get_current_time()
		{
			static result_t current = 0;
			const sieve_t tooth_index = static_cast< sieve_t >(index_.fetch_add( 1, std::memory_order_release ));
			if ( !(tooth_index & sieve ) )
				current = time_source< period, result_t >::get_current_time();
			return current;
		}

		static result_t get_time_diff( const result_t& time )
		{
			const result_t current_time = get_current_time();
			return current_time - time;
		}
	};

	template< class Period, class R >
	typename sieved_time_source< Period, R >::atomic_index_t sieved_time_source< Period, R >::index_ = 0;
	template< class Period, class R >
	typename sieved_time_source< Period, R >::sieve_t sieved_time_source< Period, R >::DEFAULT_SIEVE = 127;
	template< class Period, class R >
	typename sieved_time_source< Period, R >::sieve_t sieved_time_source< Period, R >::sieve = sieved_time_source< Period, R >::DEFAULT_SIEVE;
}

#endif // _COCURC_TIME_SOURCE_H_
