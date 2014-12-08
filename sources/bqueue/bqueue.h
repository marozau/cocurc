//  lock-free single-producer/single-consumer ringbuffer

//  this algorithm is implemented in various projects (linux kernel)

//

//  Copyright (C) 2009-2013 Tim Blechmann

//

//  Distributed under the Boost Software License, Version 1.0. (See

//  accompanying file LICENSE_1_0.txt or copy at

//  http://www.boost.org/LICENSE_1_0.txt)



#ifndef _COCURC_BQUEUE_BQUEUE_H_
#define _COCURC_BQUEUE_BQUEUE_H_



#include <algorithm>

#include <memory>



#include <boost/aligned_storage.hpp>

#include <boost/assert.hpp>

#include <boost/static_assert.hpp>

#include <boost/utility.hpp>

#include <boost/utility/enable_if.hpp>



#include <boost/type_traits/has_trivial_destructor.hpp>

#include <boost/type_traits/is_convertible.hpp>



#include <boost/lockfree/detail/atomic.hpp>

#include <boost/lockfree/detail/branch_hints.hpp>

#include <boost/lockfree/detail/copy_payload.hpp>

#include <boost/lockfree/detail/parameter.hpp>

#include <boost/lockfree/detail/prefix.hpp>



#ifdef BOOST_HAS_PRAGMA_ONCE

#pragma once

#endif



namespace boost

{

	namespace lockfree 

	{

		namespace detail   

		{

			typedef parameter::parameters<boost::parameter::optional<tag::capacity>,

				boost::parameter::optional<tag::allocator>

			> ringbuffer_signature;



			template <typename T>

			class bringbuffer_base

			{

#ifndef BOOST_DOXYGEN_INVOKED

				typedef std::size_t size_t;

				static const int padding_size = BOOST_LOCKFREE_CACHELINE_BYTES - sizeof( size_t );

				atomic<size_t> write_index_;

				char padding1[ padding_size ]; /* force read_index and write_index to different cache lines */

				atomic<size_t> read_index_;



				BOOST_DELETED_FUNCTION( bringbuffer_base( bringbuffer_base const& ) )

					BOOST_DELETED_FUNCTION( bringbuffer_base& operator= (bringbuffer_base const&) )



			protected:

				bringbuffer_base( void ) :

					write_index_( 0 ), read_index_( 0 )

				{}



				static size_t next_index( size_t arg, size_t max_size )

				{

					size_t ret = arg + 1;

					while ( unlikely( ret >= max_size ) )

						ret -= max_size;

					return ret;

				}



				static size_t read_available( size_t write_index, size_t read_index, size_t max_size )

				{

					if ( write_index >= read_index )

						return write_index - read_index;



					const size_t ret = write_index + max_size - read_index;

					return ret;

				}



				static size_t write_available( size_t write_index, size_t read_index, size_t max_size )

				{

					size_t ret = read_index - write_index - 1;

					if ( write_index >= read_index )

						ret += max_size;

					return ret;

				}



				size_t read_available( size_t max_size ) const

				{

					size_t write_index = write_index_.load( memory_order_relaxed );

					const size_t read_index = read_index_.load( memory_order_relaxed );

					return read_available( write_index, read_index, max_size );

				}



				size_t write_available( size_t max_size ) const

				{

					size_t write_index = write_index_.load( memory_order_relaxed );

					const size_t read_index = read_index_.load( memory_order_relaxed );

					return write_available( write_index, read_index, max_size );

				}



				

				template <typename Functor>

				bool push( Functor& functor, T * buffer, size_t max_size )

				{

					const size_t write_index = write_index_.load( memory_order_relaxed );  // only written from push thread

					const size_t next = next_index( write_index, max_size );



					if ( next == read_index_.load( memory_order_acquire ) )

						return false; /* ringbuffer is full */



					//new (buffer + write_index) T( t ); // copy-construct

					T & object_to_push = buffer[ write_index ];

					functor( object_to_push );



					write_index_.store( next, memory_order_release );



					return true;

				}



				template <typename Functor>

				bool push( const Functor& functor, T * buffer, size_t max_size )

				{

					const size_t write_index = write_index_.load( memory_order_relaxed );  // only written from push thread

					const size_t next = next_index( write_index, max_size );



					if ( next == read_index_.load( memory_order_acquire ) )

						return false; /* ringbuffer is full */



					//new (buffer + write_index) T( t ); // copy-construct

					T & object_to_push = buffer[ write_index ];

					functor( object_to_push );



					write_index_.store( next, memory_order_release );



					return true;

				}



				template <typename Functor>

				bool consume_one( Functor & functor, T * buffer, size_t max_size )

				{

					const size_t write_index = write_index_.load( memory_order_acquire );

					const size_t read_index = read_index_.load( memory_order_relaxed ); // only written from pop thread

					if ( empty( write_index, read_index ) )

						return false;



					T & object_to_consume = buffer[ read_index ];

					functor( object_to_consume );

					//object_to_consume.~T();



					size_t next = next_index( read_index, max_size );

					read_index_.store( next, memory_order_release );

					return true;

				}



				template <typename Functor>

				bool consume_one( const Functor & functor, T * buffer, size_t max_size )

				{

					const size_t write_index = write_index_.load( memory_order_acquire );

					const size_t read_index = read_index_.load( memory_order_relaxed ); // only written from pop thread

					if ( empty( write_index, read_index ) )

						return false;



					T & object_to_consume = buffer[ read_index ];

					functor( *object_to_consume );

					//object_to_consume.~T();



					size_t next = next_index( read_index, max_size );

					read_index_.store( next, memory_order_release );

					return true;

				}

				

				template <typename Functor>

				size_t consume_all( Functor const & functor, T * internal_buffer, size_t max_size )

				{

					const size_t write_index = write_index_.load( memory_order_acquire );

					const size_t read_index = read_index_.load( memory_order_relaxed ); // only written from pop thread



					const size_t avail = read_available( write_index, read_index, max_size );



					if ( avail == 0 )

						return 0;



					const size_t output_count = avail;



					size_t new_read_index = read_index + output_count;



					if ( read_index + output_count > max_size ) {

						/* copy data in two sections */

						const size_t count0 = max_size - read_index;

						const size_t count1 = output_count - count0;



						run_functor( internal_buffer + read_index, internal_buffer + max_size, functor );

						run_functor( internal_buffer, internal_buffer + count1, functor );



						new_read_index -= max_size;

					}

					else {

						run_functor( internal_buffer + read_index, internal_buffer + read_index + output_count, functor );



						if ( new_read_index == max_size )

							new_read_index = 0;

					}



					read_index_.store( new_read_index, memory_order_release );

					return output_count;

				}	



#endif





			public:

				/** reset the ringbuffer

				*

				* \note Not thread-safe

				* */

				void reset( void )

				{

					if ( !boost::has_trivial_destructor<T>::value ) {

						// make sure to call all destructors!



						T dummy_element;

						while ( pop( dummy_element ) )

						{

						}

					}

					else {

						write_index_.store( 0, memory_order_relaxed );

						read_index_.store( 0, memory_order_release );

					}

				}



				/** Check if the ringbuffer is empty

				*

				* \return true, if the ringbuffer is empty, false otherwise

				* \note Due to the concurrent nature of the ringbuffer the result may be inaccurate.

				* */

				bool empty( void )

				{

					return empty( write_index_.load( memory_order_relaxed ), read_index_.load( memory_order_relaxed ) );

				}



				/**

				* \return true, if implementation is lock-free.

				*

				* */

				bool is_lock_free( void ) const

				{

					return write_index_.is_lock_free() && read_index_.is_lock_free();

				}



			private:

				bool empty( size_t write_index, size_t read_index )

				{

					return write_index == read_index;

				}



				template< class Functor >

				void run_functor( T * first, T * last, Functor & functor )

				{

					for ( ; first != last; ++first ) {

						functor( *first );

					}

				}



				template< class Functor >

				void run_functor( T * first, T * last, Functor const & functor )

				{

					for ( ; first != last; ++first ) {

						functor( *first );

					}

				}



				template< class Functor >

				void run_functor_and_delete( T * first, T * last, Functor & functor )

				{

					for ( ; first != last; ++first ) {

						functor( *first );

						first->~T();

					}

				}



				template< class Functor >

				void run_functor_and_delete( T * first, T * last, Functor const & functor )

				{

					for ( ; first != last; ++first ) {

						functor( *first );

						first->~T();

					}

				}

			};



			template <typename T, std::size_t MaxSize>

			class bcompile_time_sized_ringbuffer :

				public bringbuffer_base < T >

			{

				typedef std::size_t size_type;

				static const std::size_t max_size = MaxSize + 1;



				typedef typename boost::aligned_storage < max_size * sizeof( T ),

					boost::alignment_of<T>::value

				> ::type storage_type;



				storage_type storage_;



				T * data()

				{

					return static_cast< T* >(storage_.address());

				}



			public:

				bcompile_time_sized_ringbuffer()

				{

					for ( std::size_t i = 0; i < max_size; ++i )

						new (data() + i) T(); // copy-construct

				}

				~bcompile_time_sized_ringbuffer()

				{

					for ( std::size_t i = 0; i < max_size; ++i )

						( data() + i )->~T();

				}

			protected:

				size_type max_number_of_elements() const

				{

					return max_size;

				}



			public:



				template <typename Functor>

				bool push( Functor& f )

				{

					return bringbuffer_base<T>::push( f, data(), max_size );

				}

				template <typename Functor>

				bool push( const Functor& f )

				{

					return bringbuffer_base<T>::push( f, data(), max_size );

				}



				template <typename Functor>

				bool consume_one( Functor & f )

				{

					return bringbuffer_base<T>::consume_one( f, data(), max_size );

				}



				template <typename Functor>

				bool consume_one( Functor const & f )

				{

					return bringbuffer_base<T>::consume_one( f, data(), max_size );

				}



				template <typename Functor>

				bool consume_all( Functor & f )

				{

					return bringbuffer_base<T>::consume_all( f, data(), max_size ) != 0;

				}



				template <typename Functor>

				bool consume_all( Functor const & f )

				{

					return bringbuffer_base<T>::consume_all( f, data(), max_size ) != 0;

				}

			};			

		} /* namespace detail */



	} /* namespace lockfree */

} /* namespace boost */

namespace cocurc
{
	template <typename T, std::size_t MaxSize> using bqueue = boost::lockfree::detail::bcompile_time_sized_ringbuffer< T, MaxSize >;

} /* namespace cocurc */


#endif // _COCURC_BQUEUE_BQUEUE_H_

